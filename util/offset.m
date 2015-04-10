function p = offset(s)

% p: begin positions of bins/cells/slices (starting at zero)
% s: bin/cell/slice sizes (counts)

c = cumsum(s);
p = [0 c(1:end-1)];
