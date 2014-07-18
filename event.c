#include <string.h>
#include <stdlib.h>
#include "event.h"

void EventList_add_event(EventList *self, Event event) {
    self->length++;
    self->events = realloc(self->events,
                            sizeof(Event) * self->length);
    self->events[self->length - 1] = event;
}

void EventList_destroy(EventList *self) {
    free(self->events);
}

EventList EventList_new() {
    EventList list;
    list.length = 0;
    list.events = malloc(sizeof(Event));
    list.add_event = EventList_add_event;
    list.destroy = EventList_destroy;

    return list;
}
