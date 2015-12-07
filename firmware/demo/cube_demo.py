#! /usr/bin/env python

import sys

try:
  from OpenGL.GLUT import *
  from OpenGL.GL import *
  from OpenGL.GLU import *
except:
  print '''
ERROR: PyOpenGL not installed properly.
        '''

import serial

SERIAL = '/dev/ttyUSB0'

angle = 0
pos = 0

def init():
   glClearColor (0.0, 0.0, 0.0, 0.0)
   glShadeModel (GL_FLAT)

def display():
   global angle
   global pos
   glClear (GL_COLOR_BUFFER_BIT)
   glColor3f (1.0, 1.0, 1.0)
   glLoadIdentity()             # clear the matrix
   # viewing transformation
   gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0)
   glRotate(angle,0, 1.0,0)
   glScalef(1.0, 2.0, 1.0)      # modeling transformation
   glutWireCube(1.0)
   glFlush()

def reshape (w, h):
   glViewport (0, 0, w, h)
   glMatrixMode (GL_PROJECTION)
   glLoadIdentity ()
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0)
   glMatrixMode (GL_MODELVIEW)

def keyboard(key, x, y):
   if key == chr(27):
      import sys
      sys.exit(0)


def idle():
   global angle
   line = ""
   while len(line) == 0:
     ser.write("a")
     line = ser.readline()

   angle = int(line[:-2])
   print "angle: %d" % angle



   glutPostRedisplay()



ser = serial.Serial(SERIAL, 9600, timeout=0.032)
if not ser:
  print "Unable to open serial port"
  sys.exit(1)

glutInit(sys.argv)
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB)
glutInitWindowSize (500, 500)
glutInitWindowPosition (100, 100)
glutCreateWindow ('cube')
init ()
glutDisplayFunc(display)
glutReshapeFunc(reshape)
glutKeyboardFunc(keyboard)
glutIdleFunc(idle)
glutMainLoop()
