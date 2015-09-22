# [iqm](https://github.com/iavr/iqm)

`iqm` is a temporary codename for ongoing research work. It stands for *inverted-quantized k-means* and concerns fast approximate clustering.

Licence
-------

`iqm` has a 2-clause BSD license. See file [LICENSE](/LICENSE) for the complete license text.

Directory structure
-------------------

`iqm` constists primarily of Matlab `.m` files, but also includes a number of `.cpp` files which are compiled as `mex` files to interface Matlab. The directory structure is:

	/anim      animation of algorithm iterations on 2D example
	/config    project configuration, including datasets, paths etc.
	/disp      text/graphical display
	/lib       generic utilities library
	/quant     full set of learning, encoding, inversion and search methods
	/sub       underlying code handling subspaces
	/test      main test entry points
	/util      specific utilities for this application

Requirements
------------

`iqm` requires Matlab, an appropriate C++ compiler to compile `mex` files (e.g. `gcc`) and [yael](http://yael.gforge.inria.fr/) library.

Setup
-----

After installing yael, choose a location on disk, say `home`, where to store datasets and output. Make one subfolder for each dataset with the name of the dataset, say `sift`, and copy the raw dataset files in a subfolder named `raw`. For instance, download the [SIFT1M dataset](http://corpus-texmex.irisa.fr/) and unzip its contents into

	home/sift/raw/

`iqm` uses the specific filenames and file formats as given in the link above. A number of additional folders for output will be created automatically under `home/sift/`.

Finally, edit [/config/local.m](/config/local.m) and adjust the location of `home` and `yael` to your local settings. Now you can try any of the samples under [/test/](/test/). The `mex` files should compile automatically.
