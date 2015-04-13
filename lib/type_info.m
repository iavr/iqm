function [t, s] = type_info(name)
% information on type
%
% t: formal type name
% s: size
% name: input type name (multiple aliases)

switch name
	case {'u1','uint8',  'uc','uchar'}
		[s, t] = deal(1, 'uint8');
	case {'u2','uint16', 'us','ushort'}
		[s, t] = deal(2, 'uint16');
	case {'u4','uint32', 'u', 'uint'}
		[s, t] = deal(4, 'uint32');
	case {'u8','uint64', 'ul','ulong'}
		[s, t] = deal(8, 'uint64');
	case {'i1','int8',   'c', 'char'}
		[s, t] = deal(1, 'int8');
	case {'i2','int16',  's', 'short'}
		[s, t] = deal(2, 'int16');
	case {'i4','int32',  'i', 'int'}
		[s, t] = deal(4, 'int32');
	case {'i8','int64',  'l', 'long'}
		[s, t] = deal(8, 'int64');
	case {'f4','float32','f', 'float','single'}
		[s, t] = deal(4, 'single');
	case {'f8','float64','d', 'double'}
		[s, t] = deal(8, 'double');
	otherwise
		[s, t] = deal([]);
end
