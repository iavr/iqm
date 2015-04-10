# [ikm](https://github.com/iavr/ikm)

`ikm` is a temporary codename for ongoing research work. It stands for *inverted k-means* and concerns fast approximate clustering.

Licence
-------

`ikm` has a 2-clause BSD license. See file [LICENSE](/LICENSE) for the complete license text.

Directory structure
-------------------

`ikm` constists primarily Matlab `.m` files, but also includes a number of `.cpp` files which are compiled as `mex` file to interface Matlab. The directory structure is:

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

`ikm` requires Matlab and [yael](http://yael.gforge.inria.fr/) library.

Setup
-----

After installing yael, choose a location on disk, say `HOME`, where to store datasets and output. Make one subfolder for each dataset with the name of the dataset, say `sift`, and copy the raw dataset files in a subfolder named `raw`. For instance, download the [SIFT1M dataset](ftp://ftp.irisa.fr/local/texmex/corpus/sift.tar.gz) and unzip its contents into

	HOME/sift/raw/

`ikm` uses the specific filenames and file formats as given [here](http://corpus-texmex.irisa.fr/). A number of additional folders for output will be created automatically under `HOME/sift/`.

Finally, edit [/config/config.m](/config/config.m) and adjust the location of `yael` and `HOME` to your local settings. Now you can try any of the samples under [/test/](/test/)
