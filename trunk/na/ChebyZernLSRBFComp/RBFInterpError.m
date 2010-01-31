function error = RBFInterpError(func_str, N, epsilon, fexact, r_c, theta_c)
%ERROR = RBFINTERP(FUNC_STR, N, FEXACT, R_C, THETA_C) returns the error of RBF
%	interpolation. 


f = inline(func_str, 'r', 'theta');

[r, theta] = unigrid(N, 'r');
[x, y] = unigrid(N, 'c');

npoints = size(r, 2);

finterp = zeros(1, length(r));
for ii = 1:length(r)
  finterp(ii) = f(r(ii), theta(ii));
end

G = zeros(npoints);

for ii = 1:npoints
	G(ii, :) = exp(-epsilon^2*((x(ii)-x(:)).^2 + (y(ii)-y(:)).^2));
end

a = G \ finterp';

NN = size(r_c, 2);
MM = size(theta_c, 2);

fapp = zeros(NN, MM);

for ii = 1:NN
	for jj = 1:MM
		x_c = r_c(ii) * cos(theta_c(jj));
		y_c = r_c(ii) * sin(theta_c(jj));
		fapp(ii, jj) = sum(a' .* exp(-epsilon ^ 2 * ((x_c - x).^2 ...
			+ (y_c - y).^2)));
	end
end

error = max(max(abs(fexact - fapp)));

end