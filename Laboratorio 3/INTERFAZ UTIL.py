import tkinter as tk
from tkinter import font as tkfont
import serial
import threading
import time

PORT = 'COM3'
BAUDRATE = 9600
ser = serial.Serial(PORT, BAUDRATE, timeout=1)

root = tk.Tk()
root.title("Control de Potenciómetros y LEDs")
root.geometry("700x550")
root.resizable(False, False)
root.configure(bg='#2d3e50')  # Fondo gris oscuro

custom_font = tkfont.Font(family="Helvetica", size=12, weight="bold")
large_font = tkfont.Font(family="Helvetica", size=20, weight="bold")
button_font = tkfont.Font(family="Helvetica", size=12, weight="bold")

def crear_cuadro(root, texto, color_fondo, color_texto):
    frame = tk.Frame(root, bg=color_fondo, bd=0, relief='flat', padx=10, pady=10)
    canvas = tk.Canvas(frame, width=150, height=40, bg=color_fondo, highlightthickness=0, bd=0)
    canvas.create_text(75, 20, text=texto, font=custom_font, fill=color_texto, tag="text")
    canvas.pack()
    frame.update_idletasks()
    frame.config(width=canvas.winfo_width() + 20, height=canvas.winfo_height() + 20)
    return frame

def actualizar_potenciometros():
    ser.write(b'm')

def enviar_comando_led():
    try:
        valor = int(led_value_entry.get())
        if 0 <= valor <= 255:
            ser.write(f"n{valor}\n".encode())
            led_value_label_var.set(f"Valor de LEDs: {valor}")
        else:
            led_value_label_var.set("Valor fuera de rango")
    except ValueError:
        led_value_label_var.set("El valor ingresado no es válido")
    finally:
        led_value_entry.delete(0, tk.END)

def leer_datos_serial():
    while True:
        if ser.in_waiting > 0:
            try:
                data = ser.readline().decode('utf-8').strip()
                if data.startswith('P1:'):
                    p1_value = data.split(":")[1].strip()
                    root.after(0, lambda: actualizar_texto_canvas(p1_frame, f"P1: {p1_value}"))
                elif data.startswith('P2:'):
                    p2_value = data.split(":")[1].strip()
                    root.after(0, lambda: actualizar_texto_canvas(p2_frame, f"P2: {p2_value}"))
                elif data.startswith('Valor LEDs:'):
                    valor_leds = data.split(":")[1].strip()
                    root.after(0, lambda: led_value_label_var.set(f"Valor de LEDs: {valor_leds}"))
            except UnicodeDecodeError:
                pass
        time.sleep(0.1)

def actualizar_texto_canvas(frame, texto):
    canvas = frame.winfo_children()[0]
    canvas.delete("text")
    canvas.create_text(canvas.winfo_width() // 2, canvas.winfo_height() // 2, text=texto, font=custom_font, fill='#f0f0f0', tag="text")  # Texto blanco

def crear_boton(root, text, comando, color_fondo, color_texto, size):
    boton = tk.Button(root, text=text, command=comando, font=button_font, bg=color_fondo, fg=color_texto, relief='flat', width=size, height=size, padx=0, pady=0)
    return boton

# Frame para el valor de LEDs (en el medio)
leds_frame = tk.Frame(root, bg='#2d3e50')  # Fondo gris oscuro
leds_frame.pack(pady=20, fill=tk.X)

# Label y Entry para LEDs
led_value_label_var = tk.StringVar(value="Valor de LEDs: 0")
led_value_label = tk.Label(leds_frame, textvariable=led_value_label_var, font=custom_font, bg='#f39c12', fg='#ffffff', padx=10, pady=5, relief='flat')  # Fondo naranja
led_value_label.pack(pady=(10, 0), padx=50, fill=tk.X)

led_value_entry = tk.Entry(leds_frame, font=large_font, width=20, bd=0, relief='flat', highlightthickness=2, highlightcolor='#f39c12')
led_value_entry.pack(pady=20, padx=50, fill=tk.X)

# Frame para los cuadros de potenciómetros (en la parte inferior)
potenciometros_frame = tk.Frame(root, bg='#2d3e50')  # Fondo gris oscuro
potenciometros_frame.pack(pady=20, fill=tk.X)

p1_frame = crear_cuadro(potenciometros_frame, "P1: 0", '#34495e', '#f0f0f0')  # Fondo gris más claro
p2_frame = crear_cuadro(potenciometros_frame, "P2: 0", '#34495e', '#f0f0f0')  # Fondo gris más claro

p1_frame.grid(row=0, column=0, padx=20, pady=10, sticky="nsew")
p2_frame.grid(row=0, column=1, padx=20, pady=10, sticky="nsew")

potenciometros_frame.grid_columnconfigure(0, weight=1)
potenciometros_frame.grid_columnconfigure(1, weight=1)

# Frame para los botones (en la parte inferior)
buttons_frame = tk.Frame(root, bg='#2d3e50')
buttons_frame.pack(pady=10, side=tk.BOTTOM, fill=tk.X)

button_pixel_size = 5
send_led_button = crear_boton(buttons_frame, "LEDS", enviar_comando_led, '#5a9', '#f0f0f0', button_pixel_size)  # Verde
update_pots_button = crear_boton(buttons_frame, "POTS", actualizar_potenciometros, '#5a9', '#f0f0f0', button_pixel_size)  # Verde

buttons_frame.grid_columnconfigure(0, weight=1)
buttons_frame.grid_columnconfigure(1, weight=1)

send_led_button.grid(row=0, column=0, padx=10, pady=10, sticky="ew")
update_pots_button.grid(row=0, column=1, padx=10, pady=10, sticky="ew")

def iniciar_thread():
    thread = threading.Thread(target=leer_datos_serial, daemon=True)
    thread.start()

iniciar_thread()

root.mainloop()