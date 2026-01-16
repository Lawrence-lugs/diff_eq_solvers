from os import wait
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import seaborn as sns
import numpy as np
import argparse
import ctypes
from pathlib import Path

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
    
    return array.reshape(shape).T.transpose(2,0,1)

if __name__ == "__main__":

    results_dir = Path("results")

    if not results_dir.exists():
        print("Results dir does not exist.")
        raise(FileNotFoundError)

    bin_files = list(results_dir.glob("*.bin"))

    if not bin_files:
        print("No bin files found")
        raise(FileNotFoundError)

    fig, ax = plt.subplots()

    res_array = read_array_bin(bin_files[0])
    print(res_array)


    # Reference Line
    #t = np.linspace(start=0.,stop=5e-3*500,num=500)
    #ref_x_pos = 10*t;
    #ref_y_pos = (-9.81*t*t)/2 + 10*t 
    #ref_line = ax.plot(ref_x_pos,ref_y_pos, label=f'Analytical (no drag)',ls='--')

#    sim_data = []
#    max_frames = 0
#
#    for file_path in bin_files:
#        res_array = read_array_bin(file_path)
#
#        x_pos = res_array[0]
#        y_pos = res_array[1]
#
#        current_frames = res_array.shape[1]
#        if current_frames > max_frames:
#            max_frames = current_frames
#
#        line, = ax.plot(x_pos[0], y_pos[0], label=file_path.stem)
#
#        sim_data.append({
#            'line': line,
#            'x_data': x_pos,
#            'y_data': y_pos,
#            'num_frames': current_frames
#        })
#
#    ax.set(xlim=[0,20],ylim=[-5.5,5.5],xlabel='x', ylabel='y')
#    ax.legend()
#
#    def update(frame):
#        artists = []
#        for sim in sim_data:
#            idx = min(frame, sim['num_frames']-1)
#
#            x = sim['x_data'][:idx]
#            y = sim['y_data'][:idx]
#            sim['line'].set_xdata(x)
#            sim['line'].set_ydata(y)
#            artists.append(sim['line'])
#
#        return artists
#
#    dt = 5e-3
#    interval = dt # Delay between frames
#
#    ani = animation.FuncAnimation(fig=fig, func=update, frames=max_frames, interval=interval)
#    plt.show()

    #ani.save(f'results/diffusion.gif',fps=60)
    
