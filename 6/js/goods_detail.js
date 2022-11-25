/*全局变量*/
var gwidth = 360; //商品图片宽度
var gheight = 360; //商品图片高度
var image2 = new Image(); //放大镜的图像源
image2.src = "img/detail/1.jpg";
/**商品详情*/
$(function() {

		var c = document.getElementById("canvas1");
		var ctx = c.getContext("2d");
		var img = document.getElementById("first_img");
		ctx.drawImage(img, 0, 0);
		//换图
		$("#ULlist  li").mouseover(function() {

			var index = $("#ULlist li").index(this) + 1;

			var c = document.getElementById("canvas1");
			var ctx = c.getContext("2d");
			ctx.clearRect(0, 0, gwidth, gheight); //擦除画布1中原图像
			var img = $(this).find("img");
			ctx.drawImage(img[0], 0, 0, gwidth, gheight);
			//设置放大镜的图像源
			image2 = img[0];
		});
		initCanvas1();
	})
	
/*放大镜*/
function initCanvas1() {
	var canvas1 = document.getElementById('canvas1'); //获取显示原图的canvas元素
	if(canvas1 == null)
		return false;
	context = canvas1.getContext('2d'); //获取显示原图的canvas元素的图形上下文对象
	//获取图像源
	var image = new Image();
	image.src = "img/detail/1.jpg";
	//绘制原图
	image.onload = function() {
		context.drawImage(image, 0, 0);
	}
	canvas1.onmousemove = canvas1_onmouse_move; //添加原图像获取鼠标焦点时的处理函数
	canvas1.onmouseout = canvas1_onmouse_out; //添加原图像失去鼠标焦点时的处理函数
}
//鼠标在大图上移动时的处理函数
function canvas1_onmouse_move(ev) {
	var canvas1, canvas2; 
	var x, y; //鼠标在canvas元素中的相对坐标点
	
	canvas1 = document.getElementById("canvas1"); //获取原图像使用的canvas元素
	canvas2 = document.getElementById("canvas2"); //获取放大镜中图像使用的canvas元素
	var cxt2 = canvas2.getContext('2d'); //获取放大镜中图像使用的canvas元素的图形上下文对象
	canvas2.style.display = "inline"; //显示放大镜
	cxt2.clearRect(0, 0, canvas2.width, canvas2.height); //擦除放大镜中原图像
	x = ev.offsetX;//获取鼠标在原图上的相对x坐标 
	y = ev.offsetY; //获取鼠标在原图上的相对y坐标
	canvas2.style.left = ev.clientX +5+ "px"; //设置放大镜的左边距，距离鼠标的坐标加5，防止鼠标移动到放大镜上
	canvas2.style.top = ev.clientY +5+ "px"; //设置放大镜的上边距，距离鼠标的坐标加5，防止鼠标移动到放大镜上
	//放大2倍绘制放大镜图像
	cxt2.drawImage(canvas1, x-50, y-50, 100, 100, 0, 0, 200, 200);
}
//鼠标移出原图像外
function canvas1_onmouse_out() {
	var canvas2 = document.getElementById("canvas2"); //获取放大镜所用canvas元素
	//重置canvas元素的位置
	canvas2.style.left = "0px";
	canvas2.style.top = "0px";
	//隐藏放大镜
	canvas2.style.display = "none";
}

//加入购物车
function addCart(bt) {
	var str = localStorage.getItem("cartlist");
	var json = JSON.parse(str);

	if(json == null) {
		json = {
			"list": []
		};

	}
	var data = new Object;
	data.gname = document.getElementById("gname").innerHTML;
	price = document.getElementById("price").innerHTML;
	data.price = price.substring(1);
	data.gid = document.getElementById("gid").value;
	var path = document.getElementById("first_img").src;
	var n = path.lastIndexOf("/") + 1;
	data.pic = path.substring(n);
	json.list.push(data);
	var str = JSON.stringify(json);

	localStorage.setItem("cartlist", str);
	alert("添加成功");
}

