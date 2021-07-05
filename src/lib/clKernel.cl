#pragma OPENCL EXTENSION cl_khr_fp64 : enable

__kernel void calculation(__global int* fractal, double posx, double posy, double scale, int height, int width, int iterations)
{
    int i = get_global_id(0);

    double cx = posx + scale * (double)(i % width);
    double cy = posy - scale * (double)(i / width);
    double ccx;
    double cx1 = 0.0, cy1 = 0.0;
    int j;
    for (j = 0; j < iterations; j++)
    {
        ccx = cx1;
        cx1 = cx1 * cx1 - cy1 * cy1 + cx;
        cy1 = cy1 * ccx * 2 + cy;

        if ((cx1 * cx1 + cy1 * cy1) > 4)
        {
            break;
        }
    }
    fractal[i] = j;
}
