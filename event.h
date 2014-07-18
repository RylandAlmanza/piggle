#ifndef EVENT_H
#define EVENT_H

typedef enum EventTypeEnum EventType;

enum EventTypeEnum {
    KEYDOWN
};

typedef enum EventValueEnum EventValue;

enum EventValueEnum {
    KEY_UP,
    KEY_RIGHT,
    KEY_DOWN,
    KEY_LEFT
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

EventList EventList_new();

#endif
