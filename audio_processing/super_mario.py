

import numpy as np
import pretty_midi

MIDI_FILE = "super_mario_2.mid"
OUTPUT_HEADER = "mario_melody_2_stop.h"

# ---- silence threshold: gap exceeds how many seconds to count as rest ----
SILENCE_THRESHOLD = 0.000 

# ---- MIDI note → frequency ----
def midi_to_freq(midi_note):
    return 440.0 * (2 ** ((midi_note - 69) / 12.0))

# ---- read MIDI ----
pm = pretty_midi.PrettyMIDI(MIDI_FILE)

melody_notes = []
melody_freqs = []
melody_durations = []

# ---- find target instrument----
notes = []
for instrument in pm.instruments:
    if instrument.program == 80:  
        print("Using Piano Track")
        notes = instrument.notes
        break

# ---- sort by start time ----
notes = sorted(notes, key=lambda n: n.start)

# ---- start parsing ----
for i in range(len(notes)):
    note = notes[i]

    # current note
    pitch = note.pitch
    dur_ms = int((note.end - note.start) * 1000)

    melody_notes.append(pitch)
    melody_freqs.append(midi_to_freq(pitch))
    melody_durations.append(dur_ms)

    # check gap
    if i < len(notes) - 1:
        next_note = notes[i+1]
        gap = next_note.start - note.end

        if gap > SILENCE_THRESHOLD:
            rest_ms = int(gap * 1000)

            # 插入休止符（pitch=0 表示 silence）
            melody_notes.append(0)
            melody_freqs.append(0)
            melody_durations.append(rest_ms)

# ---- output info ----
valid_notes = [n for n in melody_notes if n > 0]

min_midi = min(valid_notes)
max_midi = max(valid_notes)

print("========== Parsing Done ==========")
print("MIDI Notes:", melody_notes)
print("Total count:", len(melody_notes))
print("Durations:", melody_durations)
print("Min MIDI:", min_midi)
print("Max MIDI:", max_midi)

# ---- output header ----
with open(OUTPUT_HEADER, "w") as f:
    f.write("#ifndef MARIO_MELODY_H\n#define MARIO_MELODY_H\n\n")

    f.write("const uint8_t melody[] = {\n")
    for m in melody_notes:
        f.write(f" {m},")
    f.write("};\n\n")

    f.write("const uint16_t duration[] = {\n")
    for d in melody_durations:
        f.write(f" {d},")
    f.write("};\n\n")

    f.write("const uint16_t melody_length = " + str(len(melody_notes)) + ";\n\n")
    f.write("const uint8_t min_MIDI = " + str(min_midi) + ";\n\n")
    f.write("const uint8_t max_MIDI = " + str(max_midi) + ";\n\n")
    f.write("#endif\n")

print("Generated:", OUTPUT_HEADER)
