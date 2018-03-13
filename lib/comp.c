#include <llist.h>
#include <comp.h>

enum note {
    C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, A3, AS3, B3,
    C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, A4, AS4, B4,
    C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, A5, AS5, B5
};

struct chord {
    note_t *notes;
};

// Musical composition
// Will probably only support common time (4/4) initially
struct comp {
    int bpm;
    llist *chords;
};
