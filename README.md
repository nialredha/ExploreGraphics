# ExploreGraphics
Windows application that displays a distorted, rotating sphere made using a signed distance function / ray marching

## Overview
I was curious about graphics and the math involved, so I started a shadertoy account (https://www.shadertoy.com/user/nialred)
and played around with signed distance functions. After making a shader I liked, I wanted to find out how well it could run on one CPU core...

I also wrote some code to generate a sphere normal map in a .ppm file, which was part of a tutorial. I learned a lot from it, but the results 
aren't nearly as interesting as porting shadertoy code to C (in my opinion).

## Requirements
As you probably have realized, this was written for Windows. 

Although you are free to download the code and make your own builds, the executable is titled Sphere.exe and is located in the build folder.
After downloading, double click and enjoy a low quality moving graphic :)
