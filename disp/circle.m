function h = circle(center, radius, style, points)
% plot circle

if nargin < 3, style = 'b-'; end
if nargin < 4, points = 1000; end

z = radius * exp(sqrt(-1) * linspace(0, 2*pi, points));
h = plot(real(z) + center(1), imag(z) + center(2), style);
