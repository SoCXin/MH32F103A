# [YOLOV3](https://github.com/tfzoo/YOLOV3)

### [YOLOV3简介](https://github.com/tfzoo/YOLOV3/wiki)

YOLOV3没有太多的创新，主要是借鉴一些好的方案融合到YOLO里面。不过效果还是不错的，在保持速度优势的前提下，提升了预测精度，尤其是加强了对小物体的识别能力。

#### 主要改进

调整了网络结构；利用多尺度特征进行对象检测；对象分类用Logistic取代了softmax。

在基本的图像特征提取方面，YOLO3采用了称之为Darknet-53的网络结构（含有53个卷积层），它借鉴了残差网络residual network的做法，在一些层之间设置了快捷链路（shortcut connections）。

预测对象类别时不使用softmax，改成使用logistic的输出进行预测。这样能够支持多标签对象（比如一个人有Woman 和 Person两个标签）。


YOLO3借鉴了残差网络结构，形成更深的网络层次，以及多尺度检测，提升了mAP及小物体检测效果。如果采用COCO mAP50做评估指标（不是太介意预测框的准确性的话），YOLO3的表现相当惊人，在精确度相当的情况下，YOLOv3的速度是其它模型的3、4倍。


[![sites](tfzoo/tfzoo.png)](http://www.tfzoo.com)



