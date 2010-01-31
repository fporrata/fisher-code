function [wquad,xquad]=TN_INTatroots(N,a,b);% evaluated int_{a}^{b} f(x) dx using quadrature points on % the roots grid where [a, b] is the interval and N is the % number of interpolation points. % step one: interpolate f(x) on the roots grid.% step two: integrate term-by-erm%a = CHEEXP Mat* fg(1)=2;    % int_{-1}^{1} T_0(x) dx% g(j) are integrals of T_{j-1} from -1 to 1.for j = 3:2:N	nn=j-1;	g(j) = 1/(nn+1) - 1/(nn-1);end % j iiiia=1:2:(2*N-1) ;t=iiiia*pi/(2*N) ;xquad=0.5*(b-a)*cos(t) + 0.5*(b+a);CHEEXPMat=zeros(N,N);for ii=1:Nfor j=1:NCHEEXPMat(ii,j)=(2/N)*cos((ii-1)*t(j));end % jend % iifor j=1:NCHEEXPMat(1,j)=CHEEXPMat(1,j)*0.5;end % iifor j=1:N	wquad(j) = 0;        for k=1:2:N	wquad(j) = wquad(j) + g(k) * CHEEXPMat(k,j);       end % kend % j% adjustment for length of integration interval.wquad = 0.5 * (b-a) * wquad;