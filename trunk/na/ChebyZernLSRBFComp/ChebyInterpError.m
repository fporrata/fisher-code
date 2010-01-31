function error = ChebyInterpError(func_str, N, M, fexact, r_c, theta_c)
%ERROR = CHEBYINTERP(FUNC_STR, N, M, FEXACT, R, THETA) returns the
%	interpolation errors. FUNC_STR defines the function of interest. N and
%	M are the number of points on radius and angle, respectively. FEXACT is
%	the exact value of function on the checking grid. R_C and THETA_C
%	defines the checking grid.

f = inline(func_str, 'r', 'theta');

b1 = 'ChebQuad(r, theta, j, m)';
b2 = 'cheby(r, theta, j, m)';

%basis = inline(b2, 'r', 'theta', 'j', 'm');
basis = @cheby;

%rr = ChebyRoots(N, 0, 1);
rr = cos(pi * (2 * (1:(N-1))-1) / (4 * (N - 1)));
ttheta = 0:2*pi/M:2*pi*(1-1/M);
%rr = zeros(1, N);
%ttheta = zeros(1, M);
% for ii = 1:N-1
%     rr(ii) = cos(pi * (2*ii-1) / (4 * (N - 1)));
% end
rr(N) = 1;
% for j = 1:M
%     ttheta(j) = 2*pi*(j-1)/M;
% end

interp = zeros(N * M, 1);
for ii= 1:N
    for j = 1:M
        interp((j-1)*N+ii) = f(rr(ii), ttheta(j));
    end
end

% for ii = 1:N, for j = 1:M
%         for n = 1:N, for m = 1:M
%                 G(ii+(j-1)*N, n+(m-1)*N) = basis(rr(ii), ttheta(j),...
%                     n, m);
%             end,end
%     end,end


%Calculate the interpolation matrix
G = zeros(N, M);
for ii = 1:N, for j = 1:M
        for n = 1:N, for m = 1:(M/2 + 1)
                G(ii+(j-1)*N, n+(m-1)*N) = basis(rr(ii), ttheta(j), ...
                    2*n-1-mod(m, 2), m-1) * cos((m-1)*ttheta(j));
            end,end
        for n = 1:N, for m = 1:(M/2 - 1)
                G(ii+(j-1)*N, n+(m + M/2)*N) = basis(rr(ii), ttheta(j), ...
                    2*n-1-mod(m+1, 2), m) * sin(m*ttheta(j));
            end,end
    end,end

a = G \ interp;

NN = size(r_c, 2);
MM = size(theta_c, 2);
fapp = zeros(NN, MM);

for ii = 1:NN, for j = 1:MM
		for n = 1:N, for m = 1:(M/2 + 1)
                fapp(ii, j) =fapp(ii, j) + a(n + (m-1)*N) * basis(r_c(ii), theta_c(j),...
                    2*n-1-mod(m, 2), (m-1)) * cos((m-1)*theta_c(j));
            end,end
        for n = 1:N, for m = 1:(M/2 - 1)
                fapp(ii, j) =fapp(ii, j) + a(n + (m + M/2)*N) * basis(r_c(ii), theta_c(j), ...
                    2*n -1-mod(m+1, 2), m) * sin(m*theta_c(j));
            end,end
    end,end

error = max(max(abs(fexact - fapp)));


end