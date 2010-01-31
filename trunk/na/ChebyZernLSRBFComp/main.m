%% Main: Data preparation
clear

addpath(genpath('../../../toolbox/matlab-toolbox'));

tic

N = 25;
M = 50;

s1 = '1./(1+ r''.^2) * ones(1, length(theta))';
s2 = 'besselj(0, r'')';
s3 = 'besselj(0, 4*r'') * ones(1, length(theta))';
s4 = 'cos(10*r''*cos(theta))';
s5 = 'ChebQuad(r'', theta, 0, 0)';

s6 = 'cos(5 * r'' * cos(theta))';
s7 = 'exp(-3*(r''*cos(theta)).^2+(r''*sin(theta)).^2)./(1+r''.^2)';

s8 = '1 ./ (1 + r'' .^ 2)';
s9 = '1 ./ (1 + 3 ^ 2 * r'' .^ 2) * ones(1, length(theta))';

s10 = 'besselj(20, 20 * r'') * cos(20 * theta)';
s11 = 'besselj(10, 10 * r'') * cos(10 * theta)';

s12 = 'cos(10 * r'' * cos(theta))';

s13 = 'hills(4, r'' * cos(theta), r'' * sin(theta))';

s14 = '(4*r''.^4 - 3*r''.^2)* ones(1, length(theta))';
s15 = 'ones(length(r), 1) * ones(1, length(theta))';
s16 = 'r'' * cos(theta)';
s17 = '(2 * r'' .^ 2 - 1) * ones(1, length(theta))'; %  R_2^0
s18 = '(6 * r''.^4  - 6*r''.^2 +1) * ones(1, length(theta))'; % R_4^0
s19 = 'r''.^10 .* (-3003 + 21840 * r''.^2 - 61880 * r''.^4 + 85680 * r''.^6 - 58140 * r''.^8 + 15504 * r''.^10) * cos(10 * theta)';
s20 = 'r''.^20 .* (30045015 - 443521650 * r''.^2 + 2903050800 * r''.^4 - 11107324800 * r''.^6 + 27536909400 * r''.^8 - 46262007792 * r''.^10 + 53379239760 * r''.^12 - 41799616320 * r''.^14 + 21273019020 * r''.^16 - 6357453960 * r''.^18 + 847660528 * r''.^20) * cos(20 * theta)';
s21 = 'r''.^9 .* (-2002 + 15015 * r''.^2 - 43680 * r''.^4 + 61880 * r''.^6 - 42840 * r''.^8 + 11628 * r''.^10) * cos(9 * theta)';
func_str = s13;

f = inline(func_str, 'r', 'theta');

NN = 4 * N;
MM = 2 * M;

tr = (2*(1:NN)-1) / (2*NN);
r_c = cos(tr); %checking points on the radius
%r_c = linspace(1/NN, 1, NN);
theta_c = 2*pi*((1:MM)-1)/ MM;

fexact = zeros(NN, MM);
for ii = 1:NN
	fexact(ii, :) = f(r_c(ii), theta_c(:));
end

% The optimal epsilon values for each function are got by experiments
% described by the main script in the folder "GetOptimalepsilon"

switch func_str
	case s9
		epsilon = 5.2032;%4.9708;
	case s10
		epsilon = 5.0685;%4.7809;
	case s11
		epsilon = 2.2682;%4.2698;
	case s12
		epsilon = 2.2313;%2.2965;
	case s13
		epsilon = 1.0323;%1.0122;
	otherwise
		disp('No epsilon for the current function')
end
		
% %% Chebyshev Interpolation
tic
error = ChebyInterpError(func_str, N, M, fexact, r_c, theta_c);

disp(['Error of Chebyshev interpolation is ', num2str(error)]);

toc

% %% Logan-Shepp Interpolation
tic
error = LSInterpError(func_str, N, M, fexact, r_c, theta_c);

disp(['Error of Logan-Shepp interpolation is ', num2str(error)]);
toc

%% Zernike Interpolation
tic
[error, a] = ZernInterpError(func_str, N, M, fexact, r_c, theta_c);
disp(['Error of Zernike interpolation is ', num2str(error)]);
toc

% % %% RBF Interpolation
% tic
% error = RBFInterpError(func_str, N * M, epsilon, fexact, r_c, theta_c);
% disp(['Error of RBF interpolation is ', num2str(error)]);
% toc