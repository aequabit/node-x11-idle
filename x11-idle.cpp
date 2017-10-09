#include <node.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

int GetIdleTime () {
    time_t idle_time;
    static XScreenSaverInfo *mit_info;
    Display *display;
    int screen;
    mit_info = XScreenSaverAllocInfo();

		display = XOpenDisplay(NULL);
    if (display == NULL)
			return -1;

    screen = DefaultScreen(display);
    XScreenSaverQueryInfo(display, RootWindow(display, screen), mit_info);

    idle_time = mit_info->idle / 1000;

    XFree(mit_info);
    XCloseDisplay(display);
    return idle_time;
}

void IdleTime(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope scope(isolate);

	const unsigned argc = 0;

	int idleTime = GetIdleTime();
	if (idleTime == -1) {
		args.GetReturnValue().Set(v8::Boolean::New(isolate, false));
		return;
	}

	args.GetReturnValue().Set(v8::Integer::New(isolate, idleTime));
}

void Init(const v8::Handle<v8::Object> exports, const v8::Handle<v8::Object> module) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	exports->Set(v8::String::NewFromUtf8(isolate, "idleTime"),
	             v8::FunctionTemplate::New(isolate, IdleTime)->GetFunction());
}

NODE_MODULE(x11idle, Init)
