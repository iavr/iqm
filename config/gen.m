function cfg = gen(cfg)
% generate and save dataset

d = 2;     % # of dimensions
l = 400;    % total # of points (learn)
n = 4000;   % total # of points (base)

switch cfg.dataset
case '2d_uni'
	Y = rand(d, 4*l);
	X = rand(d, 4*n);

case '2d_gm'
	m = 8;        % # of modes
	l = l / m;    % # of points per mode (learn)
	n = n / m;    % # of points per mode (base)
	s = .1;       % max. standard deviation

	Y = [];
	X = [];
	sigma = s * rand(1,m);  % standard deviation per mode
	cen = rand(d,m);        % mean per mode
	for i = 1:m
		Y = [Y bsxfun(@plus, cen(:,i), sigma(i) * randn(d,l))];
		X = [X bsxfun(@plus, cen(:,i), sigma(i) * randn(d,n))];
	end

otherwise
	error(['Unknown dataset' cfg.dataset '.'])

end

xsave(cfg.learn, Y)
xsave(cfg.base,  X)

