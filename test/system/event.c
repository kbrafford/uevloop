#include "event.h"

#include <stdlib.h>
#include <stdint.h>

#include "system/event.h"
#include "../minunit.h"

void nop(closure_t *closure){}

static char *should_config_closure_event(){
    event_t event;
    closure_t closure = closure_create(&nop, NULL, NULL);

    event_config_closure(&event, &closure);

    mu_assert_ints_equal("event.type", CLOSURE_EVENT, event.type);
    mu_assert_pointers_equal(
        "event.closure.function",
        &nop,
        event.closure.function
    );

    return NULL;
}

static char *should_config_timer_event(){
    event_t event;
    closure_t closure = closure_create(&nop, NULL, NULL);

    uint32_t timer = 326680;
    uint16_t timeout_in_ms = 15000;

    event_config_timer(&event, timeout_in_ms, true, &closure, timer);
    mu_assert_ints_equal("event.type", TIMER_EVENT, event.type);
    mu_assert_pointers_equal(
        "event.closure.function",
        &nop,
        event.closure.function
    );
    mu_assert_ints_equal(
        "event.timer.timeout",
        timeout_in_ms,
        event.timer.timeout
    );
    mu_assert("event.timer.repeating muts had been set", event.timer.repeating);
    mu_assert_ints_equal(
        "event.timer.due_time",
        timer + timeout_in_ms,
        event.timer.due_time
    );

    return NULL;
}

static void signal_destruction(closure_t *closure){
    bool *destroyed = (bool *)closure->context;
    *destroyed = true;
}
static char *should_destroy_events(){
    {
        event_t event;
        bool destroyed = false;

        closure_t closure = closure_create(&nop, &destroyed, &signal_destruction);
        event_config_closure(&event, &closure);
        event_destroy(&event);

        mu_assert("destroyed must had been set on closure event destruction", destroyed);
    }
    {
        event_t event;
        bool destroyed = false;

        closure_t closure = closure_create(&nop, &destroyed, &signal_destruction);
        event_config_timer(&event, 1000, false, &closure, 10000);
        event_destroy(&event);

        mu_assert("destroyed must had been set on timer event destruction", destroyed);
    }

    return NULL;
}

char *event_run_tests(){
    mu_run_test(
        "should correctly config a closure event",
        should_config_closure_event
    );
    mu_run_test(
        "should correctly config a timer event",
        should_config_timer_event
    );
    mu_run_test(
        "should correctly destroy events and contained closures",
        should_destroy_events
    );

    return NULL;
}