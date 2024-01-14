#include <windows.h>

struct wqueue;
typedef void (worker)(struct wqueue *q, void *work);

struct wqueue {
    // user data pointer
    void *p;
    // lock for adding/removing work from queue.
    HANDLE lock;
    // signal for knowing when there is new work.
    HANDLE wpending;
    // new thread created for the worker.
    HANDLE thread;
    // function that will do the work.
    worker *worker;
    // work pending.
    void *work[256];
    size_t wcount;

    int closed;
};

void wstart(struct wqueue *q, worker *worker);
void wpush(struct wqueue *q, void *work);
void wclose(struct wqueue *q);
