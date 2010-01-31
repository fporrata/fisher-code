% DiskQuadPtWtTEST.m
% test routine for the numerical integration of a function
% f(r,theta) over the unit disk.
clear

M=5,   % number of points in radius
N=8,   % number of points in longitude

[w2d,ra,thetaa] = DiskQuadPtWt(M,N);

f1=inline('1+r*r*cos(theta)+r^3 * sin(3*theta)','r','theta');

I1=0;
for ii=1:M
    for j=1:N
        I1=I1 + f1(ra(ii),thetaa(j)) * w2d(ii,j);
    end %
end % ii

I1exact=pi
I1
I1relerr = (I1exact - I1)/I1exact

% Second test: orthogonality of Logan-Shepp disk polynomials
%  LS(N,j) = (1/sqrt(pi))*U_N(x cos(j*pi/(N+1)) + y*sin(j*pi/(N+1));
% where U_{N}(z)= sin((N+1) arccos(z)) / sin(arcos(z));

% size of Logan-Shepp basis is (NLS+1). The Chebyshev second
% kind polynomials are of degree (NLS).
% Exact orthogonality of the Logan-Shepp polynomials requires
% M=2*NLS+1, N=2*NLS+1

NLS=5
M=2*NLS+1,   % number of points in radius
N=2*NLS+1,   % number of points in longitude

[w2d,ra,thetaa] = DiskQuadPtWt(M,N);

for mm=1:(NLS+1)
    for nn=1:(NLS+1)
       G(mm,nn)=0;
for ii=1:M
    for j=1:N
        r=ra(ii);   theta=thetaa(j);
        x=r*cos(theta);  y=r*sin(theta);
        % argument of U_{N}(z) for the mm-th Logan-Shepp polynomial
        zmm= x*cos( (mm-1)*pi/(NLS+1)) +y*sin( (mm-1)*pi/(NLS+1));
        % argument of U_{N}(z) for the nn-th Logan-Shepp polynomial
        znn= x*cos( (nn-1)*pi/(NLS+1)) +y*sin( (nn-1)*pi/(NLS+1));
        tmm=acos(zmm);  % convert z to trigonometric argument for U_{NLS}
        LSmm= (1/sqrt(pi)) *sin( (NLS+1)*tmm)/sin(tmm);
        tnn=acos(znn);  % convert z to trigonometric argument for U_{NLS}
        LSnn= (1/sqrt(pi))*sin( (NLS+1)*tnn)/sin(tnn);
        integrand = LSmm * LSnn;
        
        G(mm,nn)=G(mm,nn) + integrand * w2d(ii,j);
    end % J
end % ii 
    end % nn
end % mm
G