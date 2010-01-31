function [r, theta] = unigrid(N, flag)
% [R, THETA] = UNIGRID(N, FLAG) returns uniform grid points on a unit circle. It
%	is not garranttee how many points are on the boundary. If FLAG is 'r',
%	then return the radial coordinate. If FLAG is 'c', return cardinal
%	coordinate.

npoints_square = ceil(N * 4.1 / pi);

npoints_side = ceil(sqrt(npoints_square));

x_side = linspace(-1, 1, npoints_side);
y_side = linspace(-1, 1, npoints_side);

r = [];
theta = [];

if flag == 'r'
for ii = 1:npoints_side
	for jj = 1:npoints_side
		length = sqrt(x_side(ii) ^ 2 + y_side(jj) ^ 2);
		if length <= 1
			r = [r length];
			theta = [theta atan2(y_side(jj), x_side(ii))];
		end
	end
end
elseif flag == 'c'
	for ii = 1:npoints_side
		for jj = 1:npoints_side
			length = sqrt(x_side(ii) ^ 2 + y_side(jj) ^ 2);
			if length <= 1
				r = [r x_side(ii)];
				theta = [theta y_side(jj)];
			end
		end
	end
else
	disp('Error: No such flag');
end