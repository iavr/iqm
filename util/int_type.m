function t = int_type(k)
% unsigned integral type able to hold up to a maximum value of k-1

if     k <= 256,        t = 'uint8';
elseif k <= 65536,      t = 'uint16';
elseif k <= 4294967296, t = 'uint32';
else                    t = 'uint64';
end
