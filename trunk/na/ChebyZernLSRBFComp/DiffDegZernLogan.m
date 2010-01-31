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

func_str = {s9, s10, s12, s13};
MaxDegree = 1:50;
errors = cell(2, 4);

for iFunc = 1:numel(func_str)
f = inline(func_str{iFunc}, 'r', 'theta');

NN = 4 * N;
MM = 2 * M;

tr = (2*(1:NN)-1) / (2*NN);
r_c = cos(tr); %checking points on the radius
%r_c = linspace(1/NN, 1, NN);
theta_c = 2*pi*((1:MM)-1)/ MM;

errors{1, iFunc} = [];
errors{2, iFunc} = [];

fexact = zeros(NN, MM);
for ii = 1:NN
	fexact(ii, :) = f(r_c(ii), theta_c(:));
end



for ii = 1:length(MaxDegree)

% Zernike Interpolation
tic
[error, a] = ZernInterpError(func_str{iFunc}, N, M, fexact, r_c, theta_c, MaxDegree(ii));
disp(['Error of Zernike interpolation is ', num2str(error)]);
toc
errors{1, iFunc} = [errors{1, iFunc}, error];

% Logan-Shepp Interpolation
tic
error = LSInterpError(func_str{iFunc}, N, M, fexact, r_c, theta_c, MaxDegree(ii));
disp(['Error of Logan-Shepp interpolation is ', num2str(error)]);
toc
errors{2, iFunc} = [errors{2, iFunc}, error];

end
end

save Zern_LS_errors errors

%%

for ii = 1:numel(func_str)
	subplot(2, 2, ii);
	hold on;
	plot(MaxDegree, log10(errors{1, ii}), 'k+');
	plot(MaxDegree, log10(errors{2, ii}), 'r-');
	hold off
	title(func_str{ii}, 'FontSize', 14);
	legend('Zernike interpolation', 'Logan-Shepp interpolation');
	xlabel('Max degree of the interpolation', 'FontSize', 14);
	ylabel('log10 of errors', 'FontSize', 14);
end
