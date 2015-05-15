% This function computes the distortion of the clustering with centers C
% on the dataset X, where the assignment of each point is in the vector I

function S = exp_computeDistortion(C, X, I)
	S = 0;
	k = size(C,2);

	for i=1:k
		tmp = sum(yael_L2sqr(C(:,i), X(:, I==i)));
		S = S + tmp;
	end
end
