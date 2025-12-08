import tkinter as tk
import serial
import threading
from queue import Queue

SERIAL_PORT = "COM6"
BAUD = 115200
THRESHOLD = 850
UPDATE_MS = 20 # 50 Hz

q = Queue()

def serial_thread():
    ser = serial.Serial(SERIAL_PORT, BAUD, timeout=0.1)
    while True:
        line = ser.readline().decode(errors="ignore").strip()
        if line:
            q.put(line)


class GridUI:
    def __init__(self, root):
        self.root = root
        root.title("Pressure Pad Visualizer")

        self.canvas = tk.Canvas(root, width=360, height=360, bg="#222")
        self.canvas.pack()

        self.blocks = []
        self.colors = ["#444"] * 9
        size = 120
        pad = 8

        for r in range(3):
            for c in range(3):
                x0 = c * size + pad
                y0 = r * size + pad
                x1 = x0 + size - pad*2
                y1 = y0 + size - pad*2
                rect = self.canvas.create_rectangle(
                    x0, y0, x1, y1,
                    fill="#444", outline="#666", width=3,
                    tags=f"block{len(self.blocks)}"
                )
                self.blocks.append(rect)

        self.update_ui()

    def update_ui(self):
        if not q.empty():
            raw = q.get()
            try:
                vals = [int(x) for x in raw.split(",")]
                for i in range(9):
                    if vals[i] > THRESHOLD:
                        self.canvas.itemconfig(self.blocks[i], fill="#33FF66")
                    else:
                        self.canvas.itemconfig(self.blocks[i], fill="#444")
            except:
                pass

        self.root.after(UPDATE_MS, self.update_ui)


threading.Thread(target=serial_thread, daemon=True).start()

root = tk.Tk()
ui = GridUI(root)
root.mainloop()
