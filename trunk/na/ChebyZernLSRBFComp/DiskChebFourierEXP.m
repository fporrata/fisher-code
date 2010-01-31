%  DiskChebFourierEXP.m
% expands an arbitrary function f(x,y)
% as a Chebyshev-Fourier series in the disk.
clear,clf
    
m=5;   % number of dof in r-direction
n=4;   % number of dof in theta-direction
       % n must be an even number


tic	   
% Does not use basis recombination
for j=1:m
tar(j)=pi*(2*j-1)/(4*(m));
ra(j)=cos(tar(j));
end %
for j=1:n
thetaa(j)=2*pi*(j-1)/n;
end %

% compute basis functions
for ii=1:m
t=tar(ii);   ss=sin(t);   cc=cos(t);
    for j=1:2*m
	rphi(ii,j)=cos((j-1)*t);
	end % j
end % ii

% Test of phi matrices


% Note that rphi, etc. are (m) x 2m matrices.
for ii=1:n
t=thetaa(ii); 
    for j=1:(n/2+1)
	cosbas(ii,j)=cos((j-1)*t);
   end % j
       for j=1:(n/2-1)
	   sinbas(ii,j)=sin(j*t);
	end % j
end % ii



% Now assemble the (mn)x(mn)  interpolation  matrix.
nge=m*n;   ncol = nge;
fa=zeros(nge,1);    G=zeros(nge,nge);

% Compute interior values of the forcing function
for ii=1:(m)
for j=1:n
r=ra(ii);
theta=thetaa(j);
x= r*cos(theta);   y=r*sin(theta);
iindex=ii + (j-1)*(m);
fa(iindex) = sqrt(2) +  x*x + y^3;  % SPECIFY f(x,y)
     
end,   % ii
end,   % j


for ii=1:m
for j=1:n
r=ra(ii);
theta=thetaa(j);
iindex=ii + (j-1)*(m);
     for icol=1:m
	   for jcol=1:(n/2+1)
	   colind=icol + (jcol-1)*m;
	   mparity=mod(jcol+1,2);
	   G(iindex,colind)= rphi(ii,2*icol-1+mparity)*cosbas(j,jcol);
	   colinda1(iindex,icol,jcol)=colind;
           end,   % jcol
           for jcol=1:(n/2-1)
	   colind=icol + (jcol+n/2)*m;
	% disp(['colind=',int2str(colind),' ii icol jcol',int2str(i),int2str(icol),int2str(jcol)])
	   mparity=mod(jcol,2);
	   G(iindex,colind)= rphi(ii,2*icol-1+mparity)*sinbas(j,jcol) ;
	    colinda2(iindex,icol,jcol)=colind;
          end,   % jcol 
     end,   %  icol
end,  % j    loop
end,  % ii   loop

a = G \ fa;

% ##################################################################
nrg=60;
ntg=60;
   u=zeros(nrg,ntg); uexact=zeros(nrg,ntg);  xg=zeros(nrg,ntg); yg=zeros(nrg,ntg);
%  waitbar(0,'Please wait; graphics is begun');
   for ii=1:nrg
	%  waitbar(ii/nrg);
	
	r=(ii-1)/(nrg-1);
	rg(ii)=r;
	t=acos(r);
	for j=1:ntg
		theta=2*pi*(j-1)/(ntg-1);
		thetag(j)=theta;
		xg(ii,j)=r*cos(theta); yg(ii,j)=r*sin(theta);
		x=xg(ii,j);   y=yg(ii,j);
		u(ii,j)=0;
		for icol=1:m
			for jcol=1:(n/2+1)
				colind=icol + (jcol-1)*m;
				if mod(jcol,2)==1
					radialbas= cos((2*icol-2)*t);
				else
					radialbas=cos((2*icol-1)*t);
				end % if
				u(ii,j)=u(ii,j) + a(colind)*cos((jcol-1)*theta)*radialbas;
			end % jcol
			for jcol=1:(n/2-1)
				colind=icol + (jcol+n/2+1-1)*m;
				if mod(jcol,2)==1
					radialbas= cos((2*icol-1)*t);
				else
					radialbas=cos((2*icol-2)*t);
				end % if
				u(ii,j)=u(ii,j) + a(colind)*sin( jcol*theta)*radialbas;
			end % jcol
		
		end % icol
		
			uexact(ii,j) =  sqrt(2) +  x*x + y^3  ;
		end,    end   % ii, j loops
		
errnorm = (max(max(abs(u-uexact))));
disp(['Maximum pointwise error=',num2str(errnorm)])




% Pack the Chebyshev-Fourier coefficients into a 2D array
for icol=1:m,   % loop over radial degree
    for jcol=1:(n/2+1)
    colind=icol + (jcol-1)*m;
    acosine2d(icol,jcol)=a(colind);
    end,   % jcol
    for jcol=1:(n/2-1)
				colind=icol + (jcol+n/2+1-1)*m;
    asine2d(icol,jcol)=a(colind);
    end, % jcol
end % icol
disp('acosine2d: row is radial index')
acosine2d
disp('asine2d: row is radial index')
asine2d

display(['disk m=radial dof  n=angular dof  '])




pcolor(xg,yg,uexact);
% surf(xg,yg,u)
colorbar
set(gca,'FontSize',18)
xlabel('x')
ylabel('y')
shading interp

totaltime=toc
