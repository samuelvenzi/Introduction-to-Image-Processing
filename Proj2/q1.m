src = imread('cookies.tif');
figure, imshow(src);
bin_img = im2bw(src, 0.3)
figure, imshow(bin_img);
SE = strel('disk',60, 0)
IM = imerode(bin_img,SE)

figure, imshow(IM);
hold on;
SE = strel('disk',70, 0)
IM2 = imdilate(IM, SE)

figure, imshow(IM2);

cookie = bitand(IM2,bin_img)

figure, imshow(cookie);

result = immultiply(cookie,src)
figure, imshow(result);
