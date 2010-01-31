% test unigrid
clear

[r, theta] = unigrid(1000, 'c');

npoints = size(r, 2);
x = zeros(1, npoints);
y = zeros(1, npoints);
for ii = 1:npoints
	x(ii) = r(ii) * cos(theta(ii));
	y(ii) = r(ii) * sin(theta(ii));
end

scatter(r, theta)