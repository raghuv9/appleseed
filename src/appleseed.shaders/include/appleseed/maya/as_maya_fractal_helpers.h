
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2016-2017 Luis Barrancos, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
 
#ifndef AS_MAYA_FRACTAL_HELPERS_H
#define AS_MAYA_FRACTAL_HELPERS_H

#include "appleseed/math/as_math_helpers.h"

void implode_2d(
    float implode,
    float implode_center[2],
    output float x,
    output float y)
{
    if (implode > EPS || implode < -EPS)
    {
        x -= implode_center[0];
        y -= implode_center[1];

        float dist = hypot(x, y);

        if (dist > EPS)
        {
            float factor = pow(dist, 1 - implode) / dist;
            x *= factor;
            y *= factor;
        }
        x += implode_center[0];
        y += implode_center[1];
    }
}

void implode_3d(
    float implode,
    float implode_center[3],
    output float x,
    output float y,
    output float z)
{
    if (implode > EPS || implode < -EPS)
    {
        x -= implode_center[0];
        y -= implode_center[1];
        z -= implode_center[2];

        float dist = hypot(x, y, z);

        if (dist > EPS)
        {
            float factor = pow(dist, 1 - implode) / dist;
            x *= factor;
            y *= factor;
            z *= factor;
        }
        x += implode_center[0];
        y += implode_center[1];
        z += implode_center[2];
    }
}

vector implode_2d(
    float implode,
    float implode_center[2],
    vector Vin)
{
    vector Vout = vector(Vin[0], Vin[1], 0.0);
    implode_2d(implode, implode_center, Vout[0], Vout[1]);
    return Vout;
}

point implode_2d(
    float implode,
    float implode_center[2],
    point Pin)
{
    point Pout = point(Pin[0], Pin[1], 0.0);
    implode_2d(implode, implode_center, Pout[0], Pout[1]);
    return Pout;
}

vector implode_3d(
    float implode,
    float implode_center[3],
    vector Vin)
{
    vector Vout = Vin;
    implode_3d(implode, implode_center, Vout[0], Vout[1], Vout[2]);
    return Vout;
}

point implode_3d(
    float implode,
    float implode_center[3],
    point Vin)
{
    point Pout = Vin;
    implode_3d(implode, implode_center, Pout[0], Pout[1], Pout[2]);
    return Pout;
}

float maya_turbulence(
    point surface_point,
    float itime,
    float filter_width,
    float amplitude,
    int octaves,
    float lacunarity,
    float gain)
{
    point pp = surface_point;
    float amp = amplitude, fw = filter_width, sum = 0.0, ttime = itime;

    for (int i = 0; i < octaves; ++i)
    {
        // Base frequency looks too regular, break it with point+noise.
        float tmp = amp * filtered_snoise(pp + noise(lacunarity), ttime, fw);
        sum += abs(tmp);
        amp *= gain;
        pp *= lacunarity;
        fw *= lacunarity;
        ttime *= lacunarity;
    }
    return sum;
}

float maya_fBm(
    point surface_point,
    float itime,
    float filter_width,
    float amplitude,
    int octaves,
    float lacunarity,
    float gain)
{
    point pp = surface_point;
    float amp = amplitude, fw = filter_width, sum = 0.0, ttime = itime;

    for (int i = 0; i < octaves; ++i)
    {
        // These magic numbers seem to match Maya better.
        sum += amp * 1.2 * (filtered_snoise(pp, ttime, fw) - 0.1) + 0.05;
        amp *= gain;
        pp *= lacunarity;
        fw *= lacunarity;
        ttime *= lacunarity;
    }
    return clamp(sum * 0.5 + 0.5, 0.0, 1.0);
}

#endif // AS_MAYA_FRACTAL_HELPERS_H
