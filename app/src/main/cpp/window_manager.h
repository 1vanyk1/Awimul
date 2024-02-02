//
// Created by Vanyk on 02.06.2021.
//

#ifndef AWIMUL_WINDOW_MANAGER_H
#define AWIMUL_WINDOW_MANAGER_H


#include "x11/headers/xlib.h"
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include "util.h"
#include "main_wm.h"

class WindowManager {
public:
    static ::std::unique_ptr<WindowManager> Create(
            const std::string& display_str = std::string());

    ~WindowManager();
    void Run();

    Display* getDisplay();

private:
    WindowManager(Display* display);
    void Frame(Window w, bool was_created_before_window_manager);
    void Unframe(Window w);
    void OnCreateNotify(const XCreateWindowEvent& e);
    void OnDestroyNotify(const XDestroyWindowEvent& e);
    void OnReparentNotify(const XReparentEvent& e);
    void OnMapNotify(const XMapEvent& e);
    void OnUnmapNotify(const XUnmapEvent& e);
    void OnConfigureNotify(const XConfigureEvent& e);
    void OnMapRequest(const XMapRequestEvent& e);
    void OnConfigureRequest(const XConfigureRequestEvent& e);
    void OnButtonPress(const XButtonEvent& e);
    void OnButtonRelease(const XButtonEvent& e);
    void OnMotionNotify(const XMotionEvent& e);
    void OnKeyPress(const XKeyEvent& e);
    void OnKeyRelease(const XKeyEvent& e);
    static int OnXError(Display* display, XErrorEvent* e);
    static int OnWMDetected(Display* display, XErrorEvent* e);
    static bool wm_detected_;
    static ::std::mutex wm_detected_mutex_;
    Display* display_;
    const Window root_;
    ::std::unordered_map<Window, Window> clients_;
    Position<int> drag_start_pos_;
    Position<int> drag_start_frame_pos_;
    Size<int> drag_start_frame_size_;

    const Atom WM_PROTOCOLS;
    const Atom WM_DELETE_WINDOW;
};


#endif //AWIMUL_WINDOW_MANAGER_H
