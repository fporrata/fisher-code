function f = hills(m, x, y)
%F = HILLS(M, X, Y) returns the value of functions representing M hills on
%	a unit circle. M defines how many hills there are. X and Y should be a
%	row vector with the first dimension being 1.

f = zeros(size(x));

for ii = 1:m
	x_temp = 0.5 * cos(ii * 2 * pi / m);
	y_temp = 0.5 * sin(ii * 2 * pi / m);
	f = f + exp(-((x - x_temp).^ 2 + (y - y_temp) .^ 2));
end