function [error, a] = ZernInterpError(func_str, N, M, fexact, r_c, theta_c, maxDegree, numQuad)

f = inline(func_str, 'r', 'theta');

if exist('maxDegree', 'var')
	NZmax = maxDegree;
else
	NZmax = 49; %Max degree of Zernike polynomials
end

if exist('numQuad', 'var')
    NZ = numQuad;
    MZ = numQuad;
else
    NZ = 2 * NZmax + 2;
    MZ = 2 * NZmax + 2;
end

[w2d_z,r_z,theta_z] = DiskQuadPtWt(NZ,MZ);

%for ii = 1:NZ
%    for j = 1:MZ
%        fgz(ii, j) = f(r_z(ii), theta_z(j));
%    end
%end

% fgz = zeros(NZ , MZ);
% for ii = 1:NZ
%   fgz(ii, :) = f(r_z(ii), theta_z);
% end
fgz = f(r_z, theta_z);

a = zeros(NZmax+1);
pz = Zern(r_z, theta_z, NZmax);
for nn = 1:(NZmax+1)
    for k = 1:nn
        a(nn, k) = 0;
        for ii = 1:NZ
            for j = 1:MZ
			  %if Zern(r_z(ii), theta_z(j), nn-1, 2*k-nn-1) ~= pz(ii, j, nn, k)
			%	disp([num2str(nn), ' ', num2str(k), ' ', num2str(ii), ' ', ...
			%		  num2str(j), ' doesn''t match'])
			 % end
%                  integrand = fgz(ii, j) * Zern(r_z(ii), theta_z(j), nn-1, 2*k-nn-1);
%                  a(nn, k) = a(nn, k) + integrand * w2d_z(ii, j);
                    integrand = fgz(ii, j) * pz(ii, j, nn, k);
                    a(nn, k) = a(nn, k) + integrand * w2d_z(ii, j);
            end
        end
        if k == (nn+1) / 2 %&& nn ~= 1
            e = 1;
        else
            e = 1/2;
        end
        a(nn, k) = a(nn, k) * nn / (e * pi);
    end
end
% a(771) = a(771) * 2;
% a(1541) = a(1541) * 2;
NN = size(r_c, 2);
MM = size(theta_c, 2);
fapp = zeros(NN, MM);
pz_c = Zern(r_c, theta_c, NZmax);

for ii = 1:NN
    for j = 1:MM
        fapp(ii, j) = 0;
        for nn = 1:(NZmax+1)
            for k = 1:nn
                fapp(ii, j) = fapp(ii, j) +...
                    ...%a(nn, k) * Zern(r_c(ii), theta_c(j), nn-1, 2*k-nn-1);
                    a(nn, k) * pz_c(ii, j, nn, k); %Zern(r_c(ii), theta_c(j), nn-1, 2*k-nn-1);
            end
        end
    end
end

error = max(max(abs(fexact - fapp)));