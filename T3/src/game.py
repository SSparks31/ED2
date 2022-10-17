import pyautogui
import pyglet

window = pyglet.window.Window(800,600)
window.clear()

WIDTH = 600
HEIGHT = 100
batch = pyglet.graphics.Batch()
lines = [pyglet.shapes.Line(x=0,y=0,x2=0,y2=0,color=(255,0,100), batch=batch), pyglet.shapes.Line(x=0,y=0,x2=0,y2=0,color=(255,0,100), batch=batch), pyglet.shapes.Line(x=0,y=0,x2=0,y2=0,color=(255,0,100), batch=batch), pyglet.shapes.Line(x=0,y=0,x2=0,y2=0,color=(255,0,100), batch=batch)]
rect = pyglet.shapes.Rectangle(x=200,y=200,width=300,height=300,color=(255,255,255))

@window.event
def on_mouse_motion(x, y, dx, dy):
    window.clear()
    rect.draw()
    lines[0].position = (x-WIDTH,y-HEIGHT,x+WIDTH,y-HEIGHT)
    lines[1].position = (x+WIDTH,y-HEIGHT,x+WIDTH,y+HEIGHT)
    lines[2].position = (x+WIDTH,y+HEIGHT,x-WIDTH,y+HEIGHT)
    lines[3].position = (x-WIDTH,y+HEIGHT,x-WIDTH,y-HEIGHT)
    batch.draw()    

pyglet.app.run()