#ifndef EVENT_H
#define EVENT_H

typedef enum EventTypeEnum EventType;

enum EventTypeEnum {
    KEYDOWN,
    KEYUP
};

typedef enum EventValueEnum EventValue;

enum EventValueEnum {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef struct EventStruct Event;

struct EventStruct {
    EventType type;
    EventValue value;
};

typedef struct EventListStruct EventList;

struct EventListStruct {
    Event *events;
    int length;
    void (*add_event)(EventList *self, Event event);
    void (*destroy)(EventList *self);
};

void event_init();

int event_value_from_key(int sdl_key);

EventList EventList_new();

#endif
