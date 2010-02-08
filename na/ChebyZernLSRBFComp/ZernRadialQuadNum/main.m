clear

addpath('..')

m = 40;
n1 = max(10, m):10:100;
n2 = n1+10;

thres = 10.^(1:-0.5:-15);
errors = ones(numel(n1), numel(thres)) * max(thres+1);
nquadpoints = zeros(size(errors));

gain = 1;

for k = 1 : numel(n1) % For each n
    tmp_nquad = 1;
    for ii = 1:numel(thres) % For each threshold
        fprintf('Loop: %d\n', ii);
        while errors(k, ii) > thres(ii)
            int = abs(zern_rad_int(n1(k), n2(k), m, tmp_nquad));
            if n1 == n2
                errors(k, ii) = abs(1 - int);
            else
                errors(k, ii) = int;
            end
            tmp_nquad = tmp_nquad + gain;
        end
        tmp_nquad = tmp_nquad - gain;
        nquadpoints(k, ii) = tmp_nquad;
    end
end

%% Draw the number of quadure points
markers = ['+', 'o', '*', '.', 'x', 's', 'd', '^', 'v', '>', '<', 'p', 'h'];
colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k'];
labels = cell(1, numel(n1));
for ii = 1:numel(labels) % Initialize labels
    labels{ii} = sprintf('n1=%d', n1(ii));
end

hold on
for ii = 1:numel(n1)
plot(-log10(thres), nquadpoints(ii, :), ['-' markers(ii) 'k']); %, 'LineWidth', 2);
end
xlabel('- log10 of the errors', 'FontSize', 14);
ylabel('Number of quadrature points needed', 'FontSize', 14);
if n1 == n2
    title(sprintf('Number of quadrature points needed to get the specific errors\n for different n when n1 = n2 and m = %d', m), 'FontSize', 16);
else
        title(sprintf('Number of quadrature points needed to get the specific errors\n for different n when n2 = n1 + %d and m = %d', n2(1)-n1(1), m), 'FontSize', 16);
end
legend(labels{:});