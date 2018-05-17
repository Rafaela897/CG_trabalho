
import sys
import math as m
import numpy as np

import re as regex
from bezierPatch import BezierPatch

filename = ""

def rotateVectorY( vector, angle ):
  M = [
    [ m.cos(angle), 0, m.sin(angle)],
    [ 0           , 1, 0           ],
    [-m.sin(angle), 0, m.cos(angle)],
  ]
  return np.dot(M, vector)


def normalizeVector( vector ):
  div = sqrt( (vector[0] ** 2) + (vector[1] ** 2) + (vector[2] ** 2) )
  vector[0] /= div
  vector[1] /= div
  vector[2] /= div
  return vector

def main( argv ) :
  if len(argv) >= 2:
    if argv[1] == "--help":
      printHelpGuide()
      return

    global filename
    filename = "../engine/code/resources/" + argv[-1]

    if argv[1] == "plane" :
      if len(argv) != 3:
        print("Wrong number or arguments. Plane takes 1 argument.")
        return
      createPlane()

    elif argv[1] == "box" :
      if len(argv) == 6 :
        createBox( float(argv[2]), float(argv[3]), float(argv[4]) )
      elif len(argv) == 7 :
        createBox( float(argv[2]), float(argv[3]), float(argv[4]), divs=int(argv[5]) )
      else:
        print( "Wrong number or arguments. Box takes 4 or 5 arguments." )
        return

    elif argv[1] == "sphere" :
      if len(argv) != 6:
        print( "Wrong number or arguments. Sphere takes 4 arguments." )
        return
      createSphere( float(argv[2]), int(argv[3]), int(argv[4]) )

    elif argv[1] == "cone" :
      if len(argv) != 7:
        print( "Wrong number or arguments. Cone takes 5 arguments." )
        return
      createCone( float(argv[2]), float(argv[3]), int(argv[4]), int(argv[5]) )

    elif argv[1] == "torus" :
      if len(argv) != 7:
        print( "Wrong number or arguments. Torus takes 5 arguments." )
        return
      createTorus( float(argv[2]), float(argv[3]), int(argv[4]), int(argv[5]) )

    elif argv[1] == "surface" :
      if len(argv) == 4:
        createBezierPatch( argv[2] )
        return
      if len(argv) == 5:
        createBezierPatch( argv[2] , int(argv[3]))
        return
      else:
        print( "Wrong number or arguments. Surface takes 2 or 3 arguments." )
        return

    return
  else:
    printHelpGuide()
    return

def printHelpGuide():
  print("\
usage: \n\
  generator --help\n\
  generator plane   <output-file>\n\
  generator box     <X-dimension> <Y-dimension> <Z-dimension> [<number-of-divisions>] <output-file>\n\
  generator sphere  <radius> <slices> <stacks> <output-file>\n\
  generator cone    <height> <bottom-radius> <slices> <stacks> <output-file>\n\
  generator torus   <big-radius> <small-radius> <slices> <stacks> <output-file>\n\
  generator surface <input-file> <output-file>\n\
  generator surface <input-file> <tesselation> <output-file>\n\
")

def printVertexNormal( x, y, z, fileObject ):
    fileObject.write( "{:.17f} {:.17f} {:.17f}\n".format(x,y,z))

def printTextures( x, y, fileObject ):
    fileObject.write( "{:.17f} {:.17f}\n".format(x,y))

def printLists( vertexList, normalList, textureList):
  global filename
  with open(filename, "w") as fileObject:
  	    fileObject.write( "\n")
  with open(filename, "w+") as fileObject:
    fileObject.write( str(len(vertexList)) + "\n" )
    for vertex in vertexList:
      printVertexNormal( vertex[0], vertex[1], vertex[2] , fileObject )
    for normal in normalList:
      printVertexNormal( normal[0], normal[1], normal[2] , fileObject )
    for texture in textureList:
      printTextures( texture[0], texture[1], fileObject )

def createPlane():
  vertexList = []
  normalList = []
  textureList = []

  # First Triangle
  vertexList.append( ( -1, 0, -1 ) )
  vertexList.append( ( -1, 0,  1 ) )
  vertexList.append( (  1, 0,  1 ) )

  normalList.append( (  0, 1,  0 ) )
  normalList.append( (  0, 1,  0 ) )
  normalList.append( (  0, 1,  0 ) )

  textureList.append( ( 0 , 0 ) )
  textureList.append( ( 0 , 1 ) )
  textureList.append( ( 1 , 1 ) )

  # Second Triangle
  vertexList.append( (  1, 0, -1 ) )
  vertexList.append( ( -1, 0, -1 ) )
  vertexList.append( (  1, 0, 1 ) )

  normalList.append( (  0, 1,  0 ) )
  normalList.append( (  0, 1,  0 ) )
  normalList.append( (  0, 1,  0 ) )

  textureList.append( ( 1 , 0 ) )
  textureList.append( ( 0 , 0 ) )
  textureList.append( ( 1 , 1) )

  printLists( vertexList, normalList, textureList)

def createBox( x, y, z, divs=1 ):

  Xfixed = x/2
  Yfixed = y/2
  Zfixed = z/2

  vertexList = []
  normalList = []
  textureList = []

  for row in range( divs ):
    for col in range( divs ):
      #frente
      thisX = x * col     / divs
      nextX = x * (col+1) / divs

      thisY = y * row     / divs
      nextY = y * (row+1) / divs

      thisZbyCol = z * col     / divs
      nextZByCol = z * (col+1) / divs

      thisZbyRow = z * row     / divs
      nextZByRow = z * (row+1) / divs

      textureX = x/(x+2*z)
      textureY = (textureX*y)/x
      textureZ = (textureX*z)/x

      # FRONT SIDE
      # Bottom Triangle
      vertexList.append( ( Xfixed - thisX ,  Yfixed - thisY ,  Zfixed ) )
      vertexList.append( ( Xfixed - nextX ,  Yfixed - nextY ,  Zfixed ) )
      vertexList.append( ( Xfixed - thisX ,  Yfixed - nextY ,  Zfixed ) )

      normalList.append( (  0, 0, 1 ) )
      normalList.append( (  0, 0, 1 ) )
      normalList.append( (  0, 0, 1 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , textureY - ((textureY*nextY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , textureY - ((textureY*nextY)/y) ) )
      # Top Triangel
      vertexList.append( ( Xfixed - thisX ,  Yfixed - thisY ,  Zfixed ) )
      vertexList.append( ( Xfixed - nextX ,  Yfixed - thisY ,  Zfixed ) )
      vertexList.append( ( Xfixed - nextX ,  Yfixed - nextY ,  Zfixed ) )

      normalList.append( (  0, 0, 1 ) )
      normalList.append( (  0, 0, 1 ) )
      normalList.append( (  0, 0, 1 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , textureY - ((textureY*nextY)/y) ) )

      # BACK SIDE
      # Bottom Triangle
      vertexList.append( ( Xfixed - thisX ,  Yfixed - thisY ,  -Zfixed ) )
      vertexList.append( ( Xfixed - thisX ,  Yfixed - nextY ,  -Zfixed ) )
      vertexList.append( ( Xfixed - nextX ,  Yfixed - nextY ,  -Zfixed ) )

      normalList.append( (  0, 0, -1 ) )
      normalList.append( (  0, 0, -1 ) )
      normalList.append( (  0, 0, -1 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (1-textureZ) - ((textureY*thisY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (1-textureZ) - ((textureY*nextY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (1-textureZ) - ((textureY*nextY)/y) ) )

      # Top Triangle
      vertexList.append( ( Xfixed - thisX ,  Yfixed - thisY ,  -Zfixed ) )
      vertexList.append( ( Xfixed - nextX ,  Yfixed - nextY ,  -Zfixed ) )
      vertexList.append( ( Xfixed - nextX ,  Yfixed - thisY ,  -Zfixed ) )

      normalList.append( (  0, 0, -1 ) )
      normalList.append( (  0, 0, -1 ) )
      normalList.append( (  0, 0, -1 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (1-textureZ) - ((textureY*thisY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (1-textureZ) - ((textureY*nextY)/y) ) )
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (1-textureZ) - ((textureY*thisY)/y) ) )

      # RIGHT SIDE
      # Bottom Triangle
      vertexList.append( ( Xfixed , Yfixed - nextY , Zfixed - nextZByCol ) )
      vertexList.append( ( Xfixed , Yfixed - thisY , Zfixed - thisZbyCol ) )
      vertexList.append( ( Xfixed , Yfixed - nextY , Zfixed - thisZbyCol ) )

      normalList.append( (  1, 0, 0 ) )
      normalList.append( (  1, 0, 0 ) )
      normalList.append( (  1, 0, 0 ) )


      textureList.append( ( 1 - ((textureZ*nextZByCol)/z) , textureY - ((textureY*nextY)/y) ) )
      textureList.append( ( 1 - ((textureZ*thisZbyCol)/z) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( 1 - ((textureZ*thisZbyCol)/z) , textureY - ((textureY*nextY)/y) ) )

      # triangulo de cima
      vertexList.append( ( Xfixed , Yfixed - thisY , Zfixed - thisZbyCol ) )
      vertexList.append( ( Xfixed , Yfixed - nextY , Zfixed - nextZByCol ) )
      vertexList.append( ( Xfixed , Yfixed - thisY , Zfixed - nextZByCol ) )

      normalList.append( (  1, 0, 0 ) )
      normalList.append( (  1, 0, 0 ) )
      normalList.append( (  1, 0, 0 ) )

      textureList.append( ( 1 - ((textureZ*thisZbyCol)/z) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( 1 - ((textureZ*nextZByCol)/z) , textureY - ((textureY*nextY)/y) ) )
      textureList.append( ( 1 - ((textureZ*nextZByCol)/z) , textureY - ((textureY*thisY)/y) ) )

      # LEFT SIDE
      # Bottom Triangle
      vertexList.append( ( - Xfixed , Yfixed - thisY , Zfixed - thisZbyCol ) )
      vertexList.append( ( - Xfixed , Yfixed - nextY , Zfixed - nextZByCol ) )
      vertexList.append( ( - Xfixed , Yfixed - nextY , Zfixed - thisZbyCol ) )

      normalList.append( (  -1, 0, 0 ) )
      normalList.append( (  -1, 0, 0 ) )
      normalList.append( (  -1, 0, 0 ) )

      textureList.append( ( textureZ - ((textureZ*thisZbyCol)/z) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( textureZ - ((textureZ*nextZByCol)/z) , textureY - ((textureY*nextY)/y) ) )
      textureList.append( ( textureZ - ((textureZ*thisZbyCol)/z) , textureY - ((textureY*nextY)/y) ) )

      # Top Triangle
      vertexList.append( ( - Xfixed , Yfixed - thisY , Zfixed - thisZbyCol ) )
      vertexList.append( ( - Xfixed , Yfixed - thisY , Zfixed - nextZByCol ) )
      vertexList.append( ( - Xfixed , Yfixed - nextY , Zfixed - nextZByCol ) )

      normalList.append( (  -1, 0, 0 ) )
      normalList.append( (  -1, 0, 0 ) )
      normalList.append( (  -1, 0, 0 ) )

      textureList.append( ( textureZ - ((textureZ*thisZbyCol)/z) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( textureZ - ((textureZ*nextZByCol)/z) , textureY - ((textureY*thisY)/y) ) )
      textureList.append( ( textureZ - ((textureZ*nextZByCol)/z) , textureY - ((textureY*nextY)/y) ) )

      # TOP SIDE
      # Bottom Triangle
      vertexList.append( ( Xfixed - thisX,  Yfixed , Zfixed - thisZbyRow ) )
      vertexList.append( ( Xfixed - thisX,  Yfixed , Zfixed - nextZByRow ) )
      vertexList.append( ( Xfixed - nextX,  Yfixed , Zfixed - nextZByRow ) )

      normalList.append( (  0, 1, 0 ) )
      normalList.append( (  0, 1, 0 ) )
      normalList.append( (  0, 1, 0 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (textureY+textureZ) - ((textureZ*thisZbyRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (textureY+textureZ) - ((textureZ*nextZByRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (textureY+textureZ) - ((textureZ*nextZByRow)/z) ))

      # Top Triangle
      vertexList.append( ( Xfixed - thisX,  Yfixed ,  Zfixed - thisZbyRow ) )
      vertexList.append( ( Xfixed - nextX,  Yfixed ,  Zfixed - nextZByRow ) )
      vertexList.append( ( Xfixed - nextX,  Yfixed ,  Zfixed - thisZbyRow ) )

      normalList.append( (  0, 1, 0 ) )
      normalList.append( (  0, 1, 0 ) )
      normalList.append( (  0, 1, 0 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (textureY+textureZ) - ((textureZ*thisZbyRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (textureY+textureZ) - ((textureZ*nextZByRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (textureY+textureZ) - ((textureZ*thisZbyRow)/z) ))

      # BOTTOM SIDE
      # Bottom Triangle
      vertexList.append( ( Xfixed - thisX,  -Yfixed , Zfixed - thisZbyRow ) )
      vertexList.append( ( Xfixed - nextX,  -Yfixed , Zfixed - nextZByRow ) )
      vertexList.append( ( Xfixed - thisX,  -Yfixed , Zfixed - nextZByRow ) )

      normalList.append( (  0, -1, 0 ) )
      normalList.append( (  0, -1, 0 ) )
      normalList.append( (  0, -1, 0 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , 1 - ((textureZ*thisZbyRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , 1 - ((textureZ*nextZByRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , 1 - ((textureZ*nextZByRow)/z) ))

      # Top Triangle
      vertexList.append( ( Xfixed - thisX,  -Yfixed , Zfixed - thisZbyRow ) )
      vertexList.append( ( Xfixed - nextX,  -Yfixed , Zfixed - thisZbyRow ) )
      vertexList.append( ( Xfixed - nextX,  -Yfixed , Zfixed - nextZByRow ) )

      normalList.append( (  0, -1, 0 ) )
      normalList.append( (  0, -1, 0 ) )
      normalList.append( (  0, -1, 0 ) )

      textureList.append( ( (1-textureZ) - ((textureX*thisX)/x) , (textureY+textureZ) - ((textureZ*thisZbyRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (textureY+textureZ) - ((textureZ*thisZbyRow)/z) ))
      textureList.append( ( (1-textureZ) - ((textureX*nextX)/x) , (textureY+textureZ) - ((textureZ*nextZByRow)/z) ))

  printLists( vertexList, normalList, textureList )

def createSphere( radius, slices, stacks ):
  thisAlpha = 0.0
  thisBeta = 0.0
  nextAlpha = 0.0
  nextBeta = 0.0

  vertexList = []
  normalList = []
  textureList = []

  def addVertex(radius, alpha, beta, slice, stack):
    vertexList.append( ( radius * m.cos(beta) * m.sin(alpha),
                          radius * m.sin(beta),
                          radius * m.cos(beta) * m.cos(alpha) ) )
    normalList.append( ( m.cos(beta) * m.sin(alpha),
                          m.sin(beta),
                          m.cos(beta) * m.cos(alpha) ) )
    textureList.append( ( slice/slices , stack/stacks) )

  # Draw Base
  stack = 0
  thisBeta = stack * (m.pi / stacks) - m.pi/2.0
  nextBeta = (stack + 1.0) * (m.pi / stacks) - m.pi/2.0
  for slice in range( slices ):
    thisAlpha = slice * (2.0*m.pi/slices)
    nextAlpha = (slice + 1.0) * (2*m.pi/slices)

    addVertex( radius, nextAlpha, nextBeta, slice+1, stack+1 )
    addVertex( radius, thisAlpha, nextBeta, slice, stack+1 )
    addVertex( radius, thisAlpha, thisBeta, slice, stack )
    # vertexList.append( ( radius * m.cos(nextBeta) * m.sin(nextAlpha),
    #                       radius * m.sin(nextBeta),
    #                       radius * m.cos(nextBeta) * m.cos(nextAlpha) ) )
    # vertexList.append( ( radius * m.cos(nextBeta) * m.sin(thisAlpha),
    #                       radius * m.sin(nextBeta),
    #                       radius * m.cos(nextBeta) * m.cos(thisAlpha) ) )
    # vertexList.append( ( radius * m.cos(thisBeta) * m.sin(thisAlpha),
    #                       radius * m.sin(thisBeta),
    #                       radius * m.cos(thisBeta) * m.cos(thisAlpha) ) )

    # textureList.append((slice+1)*(1/slices) , 1/stacks)
    # textureList.append(slice/slices, 1/stacks)
    # textureList.append(slice/slices + (1/slices)/2, 0)


  for stack in range(1, stacks -1):
    thisBeta = stack * (m.pi / stacks) - m.pi/2.0
    nextBeta = (stack + 1.0) * (m.pi / stacks) - m.pi/2.0
    for slice in range( slices ):
      thisAlpha = slice * (2.0*m.pi/slices)
      nextAlpha = (slice + 1.0) * (2*m.pi/slices)

      # First Triangle
      addVertex( radius, nextAlpha, nextBeta, slice+1, stack+1 )
      addVertex( radius, thisAlpha, nextBeta, slice, stack+1 )
      addVertex( radius, thisAlpha, thisBeta, slice, stack )
      # vertexList.append( ( radius * m.cos(nextBeta) * m.sin(nextAlpha),
      #                       radius * m.sin(nextBeta),
      #                       radius * m.cos(nextBeta) * m.cos(nextAlpha) ) )
      # vertexList.append( ( radius * m.cos(nextBeta) * m.sin(thisAlpha),
      #                       radius * m.sin(nextBeta),
      #                       radius * m.cos(nextBeta) * m.cos(thisAlpha) ) )
      # vertexList.append( ( radius * m.cos(thisBeta) * m.sin(thisAlpha),
      #                       radius * m.sin(thisBeta),
      #                       radius * m.cos(thisBeta) * m.cos(thisAlpha) ) )

      # textureList.append((slice+1)*(1/slices) , stack/stacks)
      # textureList.append(slice/slices, stack/stacks)
      # textureList.append(slice/slices, (stack+1)*(1/stacks))

      #Second Triangle
      addVertex( radius, nextAlpha, thisBeta, slice+1, stack )
      addVertex( radius, nextAlpha, nextBeta, slice+1, stack+1 )
      addVertex( radius, thisAlpha, thisBeta, slice, stack )
      # vertexList.append( ( radius * m.cos(thisBeta) * m.sin(nextAlpha),
      #                       radius * m.sin(thisBeta),
      #                       radius * m.cos(thisBeta) * m.cos(nextAlpha) ) )
      # vertexList.append( ( radius * m.cos(nextBeta) * m.sin(nextAlpha),
      #                       radius * m.sin(nextBeta),
      #                       radius * m.cos(nextBeta) * m.cos(nextAlpha) ) )
      # vertexList.append( ( radius * m.cos(thisBeta) * m.sin(thisAlpha),
      #                       radius * m.sin(thisBeta),
      #                       radius * m.cos(thisBeta) * m.cos(thisAlpha) ) )

      # textureList.append()(slice+1)*(1/slices),  (stack+1)*(1/stacks))
      # textureList.append((slice+1)*(1/slices), stack/stacks)
      # textureList.append(slice/slices, (stack+1)*(1/stacks))

  # Draw Tip
  stack = stacks-1
  thisBeta = stack * (m.pi / stacks) - m.pi/2.0
  nextBeta = (stack + 1.0) * (m.pi / stacks) - m.pi/2.0
  for slice in range( slices ):
    thisAlpha = slice * (2.0*m.pi/slices)
    nextAlpha = (slice + 1.0) * (2*m.pi/slices)

    addVertex( radius, nextAlpha, thisBeta, slice+1, stack )
    addVertex( radius, nextAlpha, nextBeta, slice, stack+1 )
    addVertex( radius, thisAlpha, thisBeta, slice, stack )
    # vertexList.append( ( radius * m.cos(thisBeta) * m.sin(nextAlpha),
    #                       radius * m.sin(thisBeta),
    #                       radius * m.cos(thisBeta) * m.cos(nextAlpha) ) )
    # vertexList.append( ( radius * m.cos(nextBeta) * m.sin(nextAlpha),
    #                       radius * m.sin(nextBeta),
    #                       radius * m.cos(nextBeta) * m.cos(nextAlpha) ) )
    # vertexList.append( ( radius * m.cos(thisBeta) * m.sin(thisAlpha),
    #                       radius * m.sin(thisBeta),
    #                       radius * m.cos(thisBeta) * m.cos(thisAlpha) ) )

    # textureList.append((slice+1)*(1/slices), 1-1/stacks)
    # textureList.append(slice/slices + (1/slices)/2, 1)
    # textureList.append(slice/slices  , 1-1/stacks)

  printLists( vertexList, normalList, textureList )

def createCone( height, radius, slices, stacks ):
  thisAlpha = 0.0
  nextAlpha = 0.0
  thisRadius = 0.0
  nextRadius = 0.0
  thisHeight = 0.0
  nextHeight = 0.0

  vertexList = []
  normalList = []
  textureList = []

  def addSideVertex( radius, alpha, height, slice, stack ):
    vertexList.append( (radius * m.sin(alpha), height, radius * m.cos(alpha)) )
    normalList.append( rotateVector(sideNormal, alpha) )
    textureList.append( ((slice/slices), 0.375 + (0.625 * stack)/stacks) )

  alpha = 2 * m.pi / slices
  stackHeight = height/stacks # "tamanho" de uma stack
  sliceRadius = radius/stacks
  # diferenca no raio entre stacks sucessivas ,
  # ou seja, a base da stack i+1 possui raio = ri - c ,
  # em que ri e o raio da base da stack anterior

  # Vertices das bases
  thisHeight = -height/2
  for slice in range( slices ):
    thisAlpha = alpha * slice
    nextAlpha = alpha * (slice + 1)

    # Base Triangle
    vertexList.append( ( 0, thisHeight, 0 ) )
    vertexList.append( ( radius * m.sin( nextAlpha ), thisHeight, radius * m.cos( nextAlpha ) ) )
    vertexList.append( ( radius * m.sin( thisAlpha ), thisHeight, radius * m.cos( thisAlpha ) ) )

    normalList.append( (0, -1, 0) )
    normalList.append( (0, -1, 0) )
    normalList.append( (0, -1, 0) )

    textureList.append((0.8125,0.1875))
    textureList.append((0.8125+0.1875*m.cos( nextAlpha) , 0.1875+0.1875*m.sin( nextAlpha)))
    textureList.append((0.8125+0.1875*m.cos( thisAlpha) , 0.1875+0.1875*m.sin( thisAlpha)))

  sideNormal = normalizeVector((0, height/2, radius/2))
  for stack in range( stacks -1 ):
    thisRadius = sliceRadius * (stacks - stack)
    nextRadius = sliceRadius * (stacks - stack-1)
    thisHeight = (stack) * stackHeight - (height/2)
    nextHeight = (stack+1) * stackHeight - (height/2)
    for slice in range( slices ):
      thisAlpha = alpha * slice
      nextAlpha = alpha * (slice + 1)

      # First Triangle
      addSideVertex(nextRadius, thisAlpha, nextHeight, slice, stack+1)
      addSideVertex(thisRadius, thisAlpha, thisHeight, slice, stack)
      addSideVertex(thisRadius, nextAlpha, thisHeight, slice+1, stack)

      # vertexList.append( ( nextRadius * m.sin( thisAlpha ), nextHeight, nextRadius * m.cos( thisAlpha )) )
      # vertexList.append( ( thisRadius * m.sin( thisAlpha ), thisHeight, thisRadius * m.cos( thisAlpha )) )
      # vertexList.append( ( thisRadius * m.sin( nextAlpha ), thisHeight, thisRadius * m.cos( nextAlpha )) )

      # textureList.append(slice/slices , 0.375 + (stack+1)*(0.625/stacks))
      # textureList.append(slice/slices , 0.375 + stack*(0.625/stacks))
      # textureList.append((slice+1)*(1/slices), 0.375 + stack*(0.625/stacks))

      # Second Triangle
      addSideVertex(thisRadius, nextAlpha, thisHeight, slice+1, stack)
      addSideVertex(nextRadius, nextAlpha, nextHeight, slice+1, stack+1)
      addSideVertex(nextRadius, thisAlpha, nextHeight, slice, stack+1)

      # vertexList.append( ( thisRadius * m.sin( nextAlpha ), thisHeight, thisRadius * m.cos( nextAlpha )) )
      # vertexList.append( ( nextRadius * m.sin( nextAlpha ), nextHeight, nextRadius * m.cos( nextAlpha )) )
      # vertexList.append( ( nextRadius * m.sin( thisAlpha ), nextHeight, nextRadius * m.cos( thisAlpha )) )

      # normalList.append( rotateVectorY(vector, nextAlpha))
      # normalList.append( rotateVectorY(vector, nextAlpha))
      # normalList.append( rotateVectorY(vector, thisAlpha))

      # textureList.append((slice+1)*(1/slices) , 0.375 + stack*(0.625/stacks))
      # textureList.append((slice+1)*(1/slices) , 0.375 + (stack+1)*(0.625/stacks))
      # textureList.append(slice/slices , 0.375 + (stack+1)*(0.625/stacks))

  # Tip
  thisRadius = sliceRadius
  thisHeight = (stacks-1) * stackHeight - (height/2)
  nextHeight = (stacks) * stackHeight - (height/2)
  for slice in range(slices):
    thisAlpha = alpha * slice
    nextAlpha = alpha * (slice + 1)
    addSideVertex(0, nextAlpha, nextHeight, slice+1, stack+1)
    addSideVertex(thisRadius, thisAlpha, thisHeight, slice, stack)
    addSideVertex(thisRadius, nextAlpha, thisHeight, slice+1, stack)
    
    # vertexList.append( (0, nextHeight, 0) )
    # vertexList.append( ( thisRadius * m.sin( thisAlpha ), thisHeight, thisRadius * m.cos( thisAlpha )) )
    # vertexList.append( ( thisRadius * m.sin( nextAlpha ), thisHeight, thisRadius * m.cos( nextAlpha )) )

    # textureList.append(0.4375,0.1875)
    # textureList.append(0.4375+0.1875*m.cos( thisAlpha) , 0.1875+0.1875*m.sin( thisAlpha))
    # textureList.append(0.4375+0.1875*m.cos( nextAlpha) , 0.1875+0.1875*m.sin( nextAlpha))
  printLists( vertexList, normalList, textureList )

def createTorus( bigRadius, smRadius, slices, stacks ):
  vertexList = []
  normalList = []
  textureList = []

  def addVertex( alpha, beta, slice, stack ):
    vertexList.append((
                      (bigRadius + smRadius * m.cos(alpha)) * m.cos(beta),
                      smRadius * m.sin(alpha),
                      (bigRadius + smRadius * m.cos(alpha)) * m.sin(beta)
                      ))
    normalList.append(( 
                      m.cos(alpha) * m.cos(beta),
                      m.sin(nextAlpha),
                      m.cos(alpha) * m.sin(beta)
                      ))
    textureList.append( (slice/slices, stack/stacks) )

  alpha = 2*m.pi / slices # ALPHA
  beta = 2*m.pi / stacks # Beta

  thisAlpha = 0.0      # TUBE radius
  nextAlpha = 0.0
  thisBeta = 0.0     # Torus radius
  nextBeta = 0.0

  # surfaceNormal = (m.cos(nextAlpha)) * m.cos(thisBeta), m.sin(nextAlpha), m.cos(nextAlpha)) * m.sin(thisBeta))

  for slice in range(slices):
    for stack in range(stacks):
      thisBeta = beta * slice            # Beta
      nextBeta = beta * (slice + 1)      # Beta

      thisAlpha = alpha * stack             # ALPHA
      nextAlpha = alpha * (stack + 1)       # ALPHA


      # First Triangle
      addVertex( nextAlpha, thisBeta, slice+1, stack)
      addVertex( thisAlpha, nextBeta, slice, stack+1)
      addVertex( thisAlpha, thisBeta, slice, stack)
      # vertexList.append(  (
      #                     (bigRadius + smRadius * m.cos(nextAlpha)) * m.cos(thisBeta),
      #                     smRadius * m.sin(nextAlpha),
      #                     (bigRadius + smRadius * m.cos(nextAlpha)) * m.sin(thisBeta)
      #                     )
      #                   )
      # vertexList.append(  (
      #                     (bigRadius + smRadius * m.cos(thisAlpha)) * m.cos(nextBeta),
      #                     smRadius * m.sin(thisAlpha),
      #                     (bigRadius + smRadius * m.cos(thisAlpha)) * m.sin(nextBeta)
      #                     )
      #                   )

      # vertexList.append(  (
      #                     (bigRadius + smRadius * m.cos(thisAlpha)) * m.cos(thisBeta),
      #                     smRadius * m.sin(thisAlpha),
      #                     (bigRadius + smRadius * m.cos(thisAlpha)) * m.sin(thisBeta)
      #                     )
      #                   )

      # textureList.append(( (slice+1)*(1/slices) , stack/stacks ))
      # textureList.append((slice/slices , (stack+1)*(1/stacks)))
      # textureList.append((slice/slices , stack/stacks ))

      # Second Triangle
      addVertex( nextAlpha, thisBeta, slice+1, stack)
      addVertex( nextAlpha, nextBeta, slice+1, stack+1)
      addVertex( thisAlpha, nextBeta, slice, stack+1)
      # vertexList.append(  (
      #                     (bigRadius + smRadius * m.cos(nextAlpha)) * m.cos(thisBeta),
      #                     smRadius * m.sin(nextAlpha),
      #                     (bigRadius + smRadius * m.cos(nextAlpha)) * m.sin(thisBeta)
      #                     )
      #                   )
      # vertexList.append(  (
      #                     (bigRadius + smRadius * m.cos(nextAlpha)) * m.cos(nextBeta),
      #                     smRadius * m.sin(nextAlpha),
      #                     (bigRadius + smRadius * m.cos(nextAlpha)) * m.sin(nextBeta)
      #                     )
      #                   )
      # vertexList.append(  (
      #                     (bigRadius + smRadius * m.cos(thisAlpha)) * m.cos(nextBeta),
      #                     smRadius * m.sin(thisAlpha),
      #                     (bigRadius + smRadius * m.cos(thisAlpha)) * m.sin(nextBeta)
      #                     )
      #                   )
      # textureList.append( (slice+1)*(1/slices) , stack/stacks )
      # textureList.append( (slice+1)*(1/slices), (stack+1)*(1/stacks))
      # textureList.append( slice/slices , (stack+1)*(1/stacks) )

      printLists( vertexList, normalList, textureList )

def createBezierPatch( inputFileName , tesselation=4 ):
  numPatches = 0
  indexes = [] # List of lists, each sublist contains the corresponding patch indexes
  controlPoints = []
  numControlPoints = 0
  line = ""
  vertexList = []
  normalList = []
  textureList = []

  """ Start reading from file and collecting information """
  with open(inputFileName, 'r') as inputFile:
    # Get number of patches
    line = inputFile.readline()
    numPatches = int(regex.split(r"[ ,\n]+", line)[0])

    # get list of indexes for each patch
    for patch in range(numPatches):
      line = inputFile.readline()
      line = line.strip(' \n') # get rid of spaces, we already have the commas to separate things.
      # This is important becouse otherwise we will get some empty strings inside our list, when separating the words.
      # regex.split(r"[ ,\n]+", line) creates a list of number strings, separated by ' ', ',' or '\n'
      # [:-1] is important because the last value in the list will be an empty string
      # int(value) for value in (...) turns the number strings int integers
      patchIndexes = [int(value) for value in regex.split(r"[ ,\n]+", line)]
      assert len(patchIndexes) == 16
      indexes.append(patchIndexes)

    assert len(indexes) == numPatches

    # Get number of control points
    line = inputFile.readline()
    numControlPoints = int(regex.split(r"[ ,\n]+", line)[0])

    for point in range(numControlPoints):
      line = inputFile.readline()
      line = line.strip(' \n')
      vertex = [float(value) for value in regex.split(r"[ ,\n]+", line)]
      controlPoints.append( (vertex[0],vertex[1],vertex[2]) )
    """ End reading from file """

    assert len(controlPoints) == numControlPoints

    """ Create bezier instances """
    bezierPatches = []
    for patch in indexes:
      P = []
      for i in range(4):
        p = []
        for j in range(4):
          pointIndex = patch[i*4 + j]
          p.append(controlPoints[pointIndex])
        P.append(p)
      bezierPatches.append(BezierPatch(P))

    def addVertex( u, v, patch ):
      vertexList.append( patch.getPointAt(u,v))
      normalList.append( patch.getNormalAt(u,v))
      textureList.append( (u,v) )

    """ Start creating vectors """
    for patch in bezierPatches:
      for i in range(tesselation):
        for j in range(tesselation):
          thisU = i/tesselation
          thisV = j/tesselation
          nextU = (i+1)/tesselation
          nextV = (j+1)/tesselation
          # First triangle
          addVertex( thisU, thisV, patch)
          addVertex( thisU, nextV, patch)
          addVertex( nextU, thisV, patch)
          # vertexList.append( patch.getPointAt(thisU,thisV) )
          # vertexList.append( patch.getPointAt(thisU,nextV) )
          # vertexList.append( patch.getPointAt(nextU,thisV) )

          # textureList.append( (0 , 1) )
          # textureList.append( (0 , 0) )
          # textureList.append( (1 , 1) )

          # Second triangle
          addVertex( thisU, nextV, patch)
          addVertex( nextU, nextV, patch)
          addVertex( nextU, thisV, patch)
          # vertexList.append( patch.getPointAt(thisU,nextV) )
          # vertexList.append( patch.getPointAt(nextU,nextV) )
          # vertexList.append( patch.getPointAt(nextU,thisV) )

          # textureList.append( 0 , 0 )
          # textureList.append( 1 , 0 )
          # textureList.append( 1 , 1 )

    printLists( vertexList, normalList, textureList )


main( sys.argv )
