function a = allocator(unit, type)

% unit: 'gpu' or 'cpu'
% type: data type ('single', 'double', etc.)

if nargin < 2, type = 'single'; end

if unit == 'gpu'

	g = gpuDevice;
	a.free  = @()   g.FreeMemory;
	a.reset = @()   reset(gpuDevice);
	a.on    = @(x)  gpuArray(x);
	a.off   = @(x)  gather(x);
	a.zeros = @(sz) gpuArray.zeros(sz, type);
	a.ones  = @(sz) gpuArray.ones(sz, type);
	a.eye   = @(sz) gpuArray.eye(sz, type);
	a.rand  = @(sz) gpuArray.rand(sz, type);
	a.randn = @(sz) gpuArray.randn(sz, type);
	a.eps   = @()   eps(type);

else

	a.free  = @()   0;
	a.reset = @()   [];
	a.on    = @(x)  x;
	a.off   = @(x)  x;
	a.zeros = @(sz) zeros(sz, type);
	a.ones  = @(sz) ones(sz, type);
	a.eye   = @(sz) eye(sz, type);
	a.rand  = @(sz) rand(sz, type);
	a.randn = @(sz) randn(sz, type);
	a.eps   = @()   eps(type);

end
