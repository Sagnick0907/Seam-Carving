# Seam-Carving
## Table of Content
  * [Demo](#demo)
  * [Problem Statement](#Problem Statement)
  * [Description](#Description)
  * [Goal](#goal)
  * [Technical Aspect](#technical-aspect)
  * [Technologies Used](#technologies-used)

## Demo
![Img CSP 1](https://user-images.githubusercontent.com/76872499/230558441-99055d5d-520d-45b2-8f59-d08ff75bb890.png)

## Problem Statement
Apply seam carving content aware image-resizing algorithm on a given image. Take the height and width (in pixels) of the output image as inputs from the user.

## Description
What is Seam Carving?  
● Seam-carving is a content-aware image resizing technique where the image
is reduced in size by one pixel of height (or width) at a time.  
● A vertical seam in an image is a path of pixels connected from the top to
the bottom with one pixel in each row.  
● A horizontal seam is a path of pixels connected from the left to the right
with one pixel in each column.  

## Motivation and Goal
Seam carving is motivated by the need for content-aware image resizing that preserves important visual elements. Its goal is to intelligently reduce or expand image size by removing or duplicating seams of low importance, minimizing distortion and maintaining visual quality.  

## Technical Aspect
Program Flow:  
1. Extract individual pixel’s RGB values from the sample image. Load the
RGB values in a 3D matrix (Height x Width x 3).  
2. Apply seam carving algorithm.  
3. Generate sample image output using the RGB values for resized image
(New_Height x New_Width x 3).

Seam Carving Steps:
○ Energy Calculation: Each pixel has some RGB values. Calculate
energy for each pixel. For ex.- You can use a dual-gradient energy
function but you are free to use any energy function of your choice.
Also, you can refer to this link for details.
○ Seam Identification: Identify the lowest energy seam.
○ Seam Removal: Remove the lowest energy seam.

## Technologies Used
- VS Code
- C++
- Libraries: OpenCV
