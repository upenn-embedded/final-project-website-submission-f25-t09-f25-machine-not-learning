import pygame
import math
import time
import threading
import serial

# ----------- CONFIG -----------
WIDTH, HEIGHT = 900, 900
BG_COLOR = (20, 20, 20)

IDLE_COLOR = (60, 60, 60)
HINT_COLOR = (255, 215, 80)
SUCCESS_COLOR = (50, 240, 140)

HIT_DURATION = 0.18
FPS = 60

NO_SIGNAL_TIMEOUT = 3.0       # reset after 3 seconds

SERIAL_PORT = "COM5"
BAUD = 115200

MUSIC_FILE = "../audio_processing/super_mario_2_bg.mid"

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Magic Mat — Super Mario")
font = pygame.font.SysFont("Arial", 40) 

# ----------- MUSIC INIT -----------
pygame.mixer.init()
pygame.mixer.music.load(MUSIC_FILE)

first_signal_received = True
last_signal_time = time.time()     # ★ track signal time

# ----------- Block Class -----------
class Block:
    def __init__(self, idx, x, y, size):
        self.idx = idx
        self.base_rect = pygame.Rect(x, y, size, size)
        self.state = "idle"
        self.hit_time = 0

    def set_state(self, state):
        self.state = state
        self.hit_time = time.time()

    def draw(self, surface):
        elapsed = time.time() - self.hit_time

        # scale animation
        if elapsed < HIT_DURATION:
            scale = 1.0 + 0.15 * (1 - elapsed / HIT_DURATION)
        else:
            scale = 1.0

        size = self.base_rect.width
        scaled = int(size * scale)
        offset = (scaled - size) // 2
        rect = pygame.Rect(
            self.base_rect.x - offset,
            self.base_rect.y - offset,
            scaled,
            scaled
        )

        if self.state == "idle":
            color = IDLE_COLOR
        elif self.state == "hint":
            color = HINT_COLOR
        elif self.state == "success":
            color = SUCCESS_COLOR

        pygame.draw.rect(surface, color, rect, border_radius=20)

        if elapsed < HIT_DURATION:
            alpha = int(255 * (1 - elapsed / HIT_DURATION))
            ring = pygame.Surface((scaled + 24, scaled + 24), pygame.SRCALPHA)
            pygame.draw.rect(
                ring,
                (color[0], color[1], color[2], alpha),
                ring.get_rect(),
                width=6,
                border_radius=20
            )
            surface.blit(ring, (rect.x - 12, rect.y - 12))

        pygame.draw.rect(surface, (0, 0, 0), rect, width=5, border_radius=20)


# ----------- Create Blocks -----------
blocks = []
size = 200                   # block size
margin = 25
start_x = (WIDTH - (size*3 + margin*2)) // 2
start_y = 120

for row in range(3):
    for col in range(3):
        idx = row*3 + col
        x = start_x + col * (size + margin)
        y = start_y + row * (size + margin)
        blocks.append(Block(idx, x, y, size))


# ----------- Reset Function -----------
def reset_to_initial():
    global first_signal_received, last_cmd

    pygame.mixer.music.stop()
    first_signal_received = True
    last_cmd = None

    for blk in blocks:
        blk.state = "idle"


# ----------- UART THREAD -----------
last_cmd = None

def uart_thread():
    global last_cmd, first_signal_received, last_signal_time

    try:
        ser = serial.Serial(SERIAL_PORT, BAUD, timeout=0.01)
        print("UART connected!")
    except:
        print("UART FAILED to open")
        return

    while True:
        b = ser.read(1)
        if not b:
            continue

        last_signal_time = time.time()   # update activity time

        if first_signal_received:
            pygame.mixer.music.play()
            first_signal_received = False

        cmd = b[0]

        if cmd == last_cmd:
            continue
        last_cmd = cmd

        if 0x00 <= cmd <= 0x08:
            block_id = cmd & 0x0F
            for blk in blocks:
                blk.state = "idle"
            blocks[block_id].set_state("hint")
            print(f"Hint: {block_id}")

        elif 0x80 <= cmd <= 0x88:
            block_id = cmd & 0x0F
            for blk in blocks:
                blk.state = "idle"
            blocks[block_id].set_state("success")
            print(f"Success: {block_id}")


threading.Thread(target=uart_thread, daemon=True).start()


# ----------- Main Loop -----------
clock = pygame.time.Clock()
running = True

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # check 2-sec no signal timeout
    if not first_signal_received:
        if time.time() - last_signal_time > NO_SIGNAL_TIMEOUT:
            print("No signal for 2 seconds: resetting...")
            reset_to_initial()

    screen.fill(BG_COLOR)

    for b in blocks:
        b.draw(screen)

    label = font.render("Magic Mat — Super Mario", True, (240, 240, 240))
    screen.blit(label, (WIDTH//2 - label.get_width()//2, 40))

    pygame.display.update()
    clock.tick(FPS)

pygame.quit()
