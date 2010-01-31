function error = LSInterpError(func_str, N, M, fexact, r_c, theta_c, maxDegree)

if exist('maxDegree', 'var')
	NLSmax = maxDegree;
else
	NLSmax = 49; %Max degree of Zernike polynomials
end
NLS = 2 * NLSmax + 2;
MLS = 2 * NLSmax + 2;

f = inline(func_str, 'r', 'theta');

[w2d_l,r_l,theta_l] = DiskQuadPtWt(NLS, MLS);

for ii = 1:NLS
    for j = 1:MLS
        fgl(ii, j) = f(r_l(ii), theta_l(j));
    end
end

    for nn=1:(NLSmax+1)
		for k=1:nn
       a(nn,k)=0;
for ii=1:MLS
    for j=1:NLS
        r=r_l(ii);   theta=theta_l(j);
        x=r*cos(theta);  y=r*sin(theta);
        % argument of U_{N}(z) for the nn-th Logan-Shepp polynomial
        zkk= x*cos( (k-1)*pi/(nn)) +y*sin( (k-1)*pi/(nn));
        tkk=acos(zkk);  % convert z to trigonometric argument for U_{NLS}
        LSkk= (1/sqrt(pi))*sin( nn*tkk)/sin(tkk);

        integrand = fgl(ii,j) * LSkk;
        
        a(nn,k)=a(nn,k) + integrand * w2d_l(ii,j);
    end % J
end % ii 
end % k
end % nn

NN = size(r_c, 2);
MM = size(theta_c, 2);
fapp = zeros(NN, MM);

for ii = 1:NN
    for j = 1:MM
        fapp(ii, j) = 0;
        for nn = 1:(NLSmax+1)
            for k = 1:nn
                        r_temp=r_c(ii);   theta_temp=theta_c(j);
        temp_x=r_temp*cos(theta_temp);  temp_y=r_temp*sin(theta_temp);
        % argument of U_{N}(z) for the nn-th Logan-Shepp polynomial
        zkk= temp_x*cos( (k-1)*pi/(nn)) +temp_y*sin( (k-1)*pi/(nn));
        tkk=acos(zkk);  % convert z to trigonometric argument for U_{NLS}
        LSkk= (1/sqrt(pi))*sin( nn*tkk)/sin(tkk);
        fapp(ii, j) = fapp(ii, j) +...
            a(nn, k) * LSkk;
            end
        end
    end
end

error = max(max(abs(fexact - fapp)));

end