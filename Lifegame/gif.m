% �˴�����������gif��ͼƬ����ӦΪ��Ȼ��˳�������� 1,2��3,4��
file_path='C:\Users\zhxing\Desktop\LifeGame\Lifegame\res\';
for i=1:100
    img_path=[file_path num2str(i) '.jpg'];
    [img,map]=imread(img_path);
    if i==1
        imwrite(img,'test.gif','Loopcount',inf,'DelayTime',0.04);
    else
        imwrite(img,'test.gif','WriteMode','append','DelayTime',0.04);
    end
end
