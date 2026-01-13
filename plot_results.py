from os import wait
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
import numpy as np
import argparse
import ctypes

def read_array_bin(filename):

    size_t = ctypes.sizeof(ctypes.c_size_t);

    if size_t == 4:
        datatype = np.uint32;
    else:
        datatype = np.uint64;

    with open(filename,'rb') as f:
        ndims = np.fromfile(f,datatype,1)[0];
        shape = np.fromfile(f,datatype,ndims); 
        array = np.fromfile(f,np.float32,shape.prod());
    
    return array.reshape(shape).T

if __name__ == "__main__":

    parser = argparse.ArgumentParser();

    parser.add_argument("input_file",help="input binary file to use");
    
    args = parser.parse_args();

    filename=f'{args.input_file[:-4]}'

    res_array = read_array_bin(args.input_file)
    print(res_array)

    x_pos = res_array[0]
    y_pos = res_array[1]

    # Reference solution
    t = np.linspace(start=0.,stop=5e-3*500,num=500)
    ref_x_pos = 10*t;
    ref_y_pos = (-9.81*t*t)/2 + 10*t 

    fig, ax = plt.subplots()
    ref_line = ax.plot(ref_x_pos,ref_y_pos, label=f'Analytical (no drag)',ls='--')
    line = ax.plot(x_pos[0], y_pos[0], label=f'Forward Euler')[0]
    
    ax.set(xlim=[0,20],ylim=[-5.5,5.5],xlabel='x', ylabel='y')
    ax.legend()

    def update(frame):
        x = x_pos[:frame]
        y = y_pos[:frame]
        line.set_xdata(x)
        line.set_ydata(y)
        return (line,line)

    dt = 5e-3
    frames = res_array.shape[1] # Total number of frames
    interval = dt # Delay between frames

    ani = animation.FuncAnimation(fig=fig, func=update, frames=frames, interval=interval)
    plt.show()

    fps = int(1/dt)
    #ani.save(f'{filename}.gif',fps=60)
    
