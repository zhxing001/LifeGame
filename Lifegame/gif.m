% 此代码用于制作gif，图片命名应为自然数顺序命名： 1,2，3,4等
file_path='C:\Users\zhxing\Desktop\LifeGame\Lifegame\res\';
for i=1:150
    i
    img_path=[file_path num2str(i) '.jpg'];
    [img,map]=imread(img_path);
    imshow(img);
    if i==1
        imwrite(img,'x.gif','Loopcount',inf,'DelayTime',0.04);
    else
        imwrite(img,'x.gif','WriteMode','append','DelayTime',0.04);
    end
end
