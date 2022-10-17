import pyautogui
import pyglet

from pyglet.shapes import *


window = pyglet.window.Window(800,600)
window.clear()

WIDTH = 600
HEIGHT = 100
cursor = Rectangle(0,0,WIDTH,HEIGHT, color=(255,0,255))
rect = Rectangle(200,200,300,300,color=(255,255,255))

def intersect(rect, cursor):
    return False

@window.event
def on_mouse_motion(x, y, dx, dy):
    window.clear()
    if intersect(rect.position, cursor.position):
        rect.color=(255,0,0)
    else:
        rect.color=(255,255,255)

    rect.draw()
    cursor.position=(x-WIDTH//2, y-HEIGHT//2)
    cursor.draw()



pyglet.app.run()