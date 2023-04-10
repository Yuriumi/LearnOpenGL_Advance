# 高级OpenGL

[TOC]

- [x] 完成**深度测试**学习,并提交Git
- [x] 完成**模板测试**学习,并提交Git
- [ ] 完成**混合**学习,并提交Git
- [ ] 完成**面剔除**学习,并提交Git
- [ ] 完成**帧缓冲**学习,并提交Git
- [ ] 完成**立方体贴图**学习,并提交Git
- [ ] 完成**高级数据**学习,并提交Git
- [ ] 完成**高级GLSL**学习,并提交Git
- [ ] 完成**几何着色器**学习,并提交Git
- [ ] 完成**实例化**学习,并提交Git
- [ ] 完成**抗锯齿**学习,并提交Git

## 深度测试 [教程页](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/01%20Depth%20testing/)

---

深度缓冲(或z缓冲(z-buffer))中的深度值(Depth Value)与颜色缓冲一样,有着一样的**宽度**和**高度**.  

深度缓冲是由窗口系统自动创建的,以多种float形式存储他的值,大部分系统中,深度缓冲的精度为**24位**.  

深度测试是在片段着色器运行之后(以及模板测试(Stencil Testing)运行之后),在屏幕空间中运行的;可以使用GLSL内建变量*GL_FragCoord*从片段着色器中直接访问;其中的**x**,**y**表示的片段的屏幕空间坐标,而**z分量**包含了片段真正的深度值,深度测试对比的就是**z分量**.  
>特性/提示  
>大部分GPU提供一个叫*提前深度测试*(Early Depth Testing)的硬件特性,允许深度测试在片段着色器之前运行,但前提是我们非常确定这个片段永远是不可见的.  
>片段着色器通常**开销很大**,应尽量避免其运行;当使用提前深度测试时,片段着色器会限制你**不能写入片段的深度值**,片段着色器对深度值进行写入,提前深度测试是不可能的,*OpenGL不能提前知道深度值*.  

深度测试默认是**禁用**的

``` C++
glEnable(GL_DEPTH_TEST);
```

深度测试启用时,片段的深度测试通过,OpenGL会丢弃原本存储的z值;未通过时,会丢弃该片段.启用深度测试,应该在每个渲染迭代前使用*GL_DEPTH_BUFFER_BIT*来清除深度缓冲,否则深度值会一直为上一次渲染迭代的值.

``` C++
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

使用深度掩码可以设置深度缓冲的**可读性**,例如我们希望开启深度测试,但不希望深度缓冲更新,让它成为一个**只读**(Read-only).

``` C++
glDepthMask(GL_FALSE);
```

--- 

### 深度测试函数  

OpenGL允许我们修改深度测试中使用的**比较运算符**(也就是让我们自己决定什么条件下更新深度缓冲),调用glDepthFunc函数来修改.

``` C++
glDepthFunc(GL_LESS);
```

| 函数 |描述 |
|--- |--- |
|GL_ALWAYS|永远通过深度测试|
|GL_NEVER|永远不通过深度测试|
|GL_LESS|在片段深度值小于缓冲深度时通过测试|
|GL_EQUAL|在片段深度值等于缓冲深度时通过测试|
|GL_LEQUAL|在片段深度值等于小于缓冲深度时通过测试|
|GL_GEQUAL|在片段深度值大于等于缓冲深度时通过测试|
|GL_GREATER|在片段深度值大于缓冲深度时通过测试|
|GL_NOEQUAL|在片段深度值不等于缓冲深度时通过测试|

默认使用*GL_LESS*,它会丢弃深度值大于等于当前深度值的片段.

### 深度测试的精度

深度缓冲区的缓冲值介于*0.0*和*1.0*之间,其值将会与平截头体内所有的对象的z缓冲进行比较;视图空间中的这些z值可以是平截头体near和far中的任何值;我们需要线性转化他们,使z缓冲位于[0.0,1.0],以下是转化线性方程

> $$F_{depth} = {z - near \over Far - near}$$

![线性方程图像](https://learnopengl-cn.github.io/img/04/01/depth_linear_graph.png)

实践中,线性深度缓冲区*几乎从未使用过*.  

> 特性/提示  
>所有的方程都会将非常近的物体的z缓冲设置为接近0.0的值,而物体非常接近远平面时,他的z缓冲将会非常接近1.0.

为了有正确的投影性质[^1],需要使用一个非线性方程,他与1/z成正比;他能做到的就是在z值很小的时候提供非常高的精度,而在z值很大的时候提供更少的精度.

> $$F_{depth} = {1/z - 1/near\over 1/far - 1-near}$$  

这个非线性方程与1/z成正比,在1.0和2.0之间的z值会变换到1.0到0.5之间的深度值,float为我们提供了一半的精度(z值小精度大),50.0到100.0之间的z值只会占用2%的float精度

只需记住z缓冲的值在屏幕空间中并不是现行的(在**透视矩阵应用之前**在**观察空间之中**是线性的).

![非线性方程图](https://learnopengl-cn.github.io/img/04/01/depth_non_linear_graph.png)

### 深度缓冲可视化

我们可以使用内建gl_FragCoord向量的z值来完成深度缓冲的可视化.

``` C++
void main()
{
    FragColor = vec4(vec3(gl_FragCoord.z),1.0f);
}
```

由于非线性方程的缘故,为了观察到我们希望的黑白渐变图像,应该拉近摄像机,使片元接近进平面.

![渲染深度图](https://learnopengl-cn.github.io/img/04/01/depth_testing_visible_depth.png)

该图很清晰的展示了深度值的非线性特质.

### 深度冲突

两个面非常紧密排列在一起会发生一个常见的视觉错误,z缓冲没有足够的精度来决定那个在前面.结果就是两个面不断地切换先后顺序,这个现象叫做**深度冲突**.

![深度冲突](https://learnopengl-cn.github.io/img/04/01/depth_testing_z_fighting.png)

深度冲突是很常见的问题,尤其是在远处的物体(z值在较大的时候拥有很小的精度).

#### 防止深度冲突

- **永远不要把多个物体摆的太近,以至于他们的一些三角形会重叠**.
- **尽可能将进平面设置的远一些**.
- **牺牲一些性能,使用精度更高的深度缓冲**.
目前有很多抗深度冲突的技术,但都不能完全解决深度冲突.

## 模板测试 [教程页](https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/02%20Stencil%20testing/)

当片段着色器处理完一个片段之后,<font color=green>模板测试</font>(Stencil Test)会开始执行,与深度测试相同的是它也会丢弃片段,被保留的片段进入深度测试;模板测试是根据*模板缓冲*(Stencil Buffer)来进行的.

一个模板缓冲中,通常每个*模板值*(Stencil Value)是**8位**的.所以每个像素可以有256钟模板值.我们可以自行设置,当某一个片段为某一个模板值时,我们可以决定**是否丢弃**它.

>特性/提示
每个窗口库都需要为你配置一个模板缓冲;GLFW自动做了这件事,但其他的窗口库不一定会.

模板缓冲案例

![模板缓冲应用案例](https://learnopengl-cn.github.io/img/04/02/stencil_buffer.png)

模板缓冲首先被清除为0,然后填充一个空心的1,场景中的片段只有模板值为1时才被渲染.

模板缓冲允许我们在**渲染片段时**将模板缓冲设定为一个特定的值;通过在**渲染时修改**模板缓冲的内容,我们写入了模板缓冲.在**同一个**(**或接下来**)渲染迭代中,这些值我们可以**读取**,来决定是否丢弃.

大体步骤:

- 启用模板缓冲的写入.
- 渲染物体,更新模板缓冲.
- 禁用模板缓冲的写入.
- 渲染(其他)物体,这次根据模板缓冲的内容丢弃特定的片段.

使用GL_STENCIL_TEST来启用模板测试

``` C++
glEnable(GL_STENCIL_TEST);
```

与颜色和深度缓冲一样,每次渲染迭代之前都应该清除缓冲

``` C++
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
```

和深度测试的glDepthMask函数一样,模板测试也有类似的函数.glStencilMask允许我们设置一个*位掩码*(*BitMask*),它会与将要写入缓冲的模板值进行**与(**AND**)运算**.默认情况下设置的所有位掩码都为1,不影响输出,但如果我们将其设置为**0x00**,写入缓冲的模板值最后都为0,这与深度测试中*glDepthMask*(*GL_FALSE*)是等价的.

``` C++
glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
glStencilMask(0x00); // 每一位在写入模板缓冲时都会被改为0(禁止写入)
```

大部分情况下只会使用**0xFF**与**0x00**作为模板掩码.

### 模板函数

和深度测试一样,模板测试应该通过还是失败,以及他应该如何影响模板缓冲,是由两个函数能够用来配置的; *glStencilFunc*和*glStencilOp*

``` C++
glStencilFunc(GLenum func, GLint ref, GLuint mask);
```

- *func*: 设置模板测试函数(Stencil Test Function).这个测试函数将会应用到已储存的模板值上和*ref*上;可用的选项有*GL_NEVER*,*GL_LESS*,*GL_LEQUAL*,*GL_GREATER*,*GL_GEQUAL*,*GL_EQUAL*,*GL_NOTEQUAL*,*GL_ALWAYS*;语义与深度缓冲类似.
- *ref*: 设置了模板测试的参考值(Rederence Value).模板缓冲的内容将会于这个值进行比较.
- *mask*: 设置一个掩码,他将会与参考值和存储的模板值在测试比较他们之前进行与(AND)运算,初始情况下所有值都为1.

上述模板测试案例中,函数被设置为:

``` C++
glStencilFunc(GL_EQUAL, 1, 0xFF);
```

这会告诉OpenGL,只要一个片段的模板值等于(GL_EQUAL)参考值1,片段将会通过测试并被绘制,否则会被丢弃.

但是*glStencilFunc*仅仅描述了OpenGL应该对模板缓冲内容做什么,但不知道应该如何更新缓冲;需要*glStencilOp*函数.

``` C++
glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
```

- **sfail**: 模板测试失败时采取的行为.
- **defail**: 模板测试通过,但深度测试失败采取的行为.
- **dppass**: 模板深度测试均通过时采取的行为.

每个选项都可以选用以下的其中一种行为：

|行为|描述|
|---|---|
|GL_KEEP|保持当前储存的模板值|
|GL_ZERO|将模板值设置为0|
|GL_REPLACE|将模板值设置为glStencilFun的**ref**值|
|GL_LNCR|如果模板值小于最大值将模板值加1|
|GL_INCR_WRAP|与GL_INCR一样,但如果模板值超过了最大值则归零|
|GL_DECR|如果模板值大于最小值则将模板值减1|
|GL_DECR_WRAP|与GL_DECR一样,但如果模板值小于0则将其设置为最大值|
|GL_INVERT|按位翻转当前的模板缓冲值|

默认情况下*glStencilOp*是设置为(*GL_KEEP,GL_KEEP,GL_KEEP*)的,所以不论测试结果如何,模板缓冲都会保留他的值.

### 物体轮廓(Object Outlining)

为每个,或一个物体在他周围创建一个有色的边框;步骤如下:

1. 在绘制(需要添加轮廓)物体之前,将模板函数设置为GL_ALWAYS,每当物体的片段被渲染时,将这个片段模板缓冲更新为1.
2. 渲染物体.
3. 禁用模板写入以及深度测试.
4. 将物体缩放一点点.
5. 使用一个不同的片段着色器,输出一个单独的(边框)颜色.
6. 再次绘制物体,但只在他们片段的模板值不等于1时才绘制.
7. 再次启用模板写入和深度测试.

个人理解:

1. 第一遍渲染物体,物体所在的屏幕空间区域模板值为1其余为0.
2. 禁用模板写入和深度测试
3. 再次绘制物体(边框),只有模板值不为1,且在物体(边框)所在的屏幕区域绘制.
4. 再次启用模板写入和深度测试。

轮廓线添加流程

``` C++
glEnable(GL_DEPTH_TEST);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

glStencilMask(0x00); // 记得保证我们在绘制地板的时候不会更新模板缓冲
normalShader.use();
DrawFloor()  

glStencilFunc(GL_ALWAYS, 1, 0xFF); 
glStencilMask(0xFF); 
DrawTwoContainers();

glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilMask(0x00); 
glDisable(GL_DEPTH_TEST);
shaderSingleColor.use(); 
DrawTwoScaledUpContainers();
glStencilMask(0xFF);
glEnable(GL_DEPTH_TEST); 
```

[^1]: **非正确投影性质**: 1.远处的物体看起来和近处的物体一样大小，**缺乏透视效果**;2.深度精度会随着深度的增加而线性减小，**导致远处的物体出现深度精度问题**；3.由于深度精度的问题，很容易出现*Z-fighting*现象，即两个物体的深度值非常接近，导致交替显示，看起来**闪烁**.
