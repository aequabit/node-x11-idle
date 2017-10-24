#include <node.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>

int GetIdleTime () {
  XScreenSaverInfo *mit_info = XScreenSaverAllocInfo();

  Display* display = XOpenDisplay(NULL);
  if (display == NULL)
		return -1;

  int screen = DefaultScreen(display);
  XScreenSaverQueryInfo(display, RootWindow(display, screen), mit_info);

  time_t idle_time = mit_info->idle / 1000;

  XFree(mit_info);
  XCloseDisplay(display);

  return idle_time;
}

void IdleTime(const v8::FunctionCallbackInfo<v8::Value>& args) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

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
