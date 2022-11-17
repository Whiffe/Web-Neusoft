window.onload = function() {
				loadAll();
			}
			//获取localStorage存储的商品信息。此时获取的是由"商品详情"页面添加到购物车的动态数据。
			//页面上前4个商品是静态效果，与此函数无关。
		function loadAll() {
			var str = localStorage.getItem("cartlist");

			var data = JSON.parse(str);
			var cart = document.getElementById("cart");
			var newForm;
			if(data.list.length > 0) {
				newForm = document.createElement("form");
				newForm.name = "shop3";
				newForm.innerHTML = "<div style=\"margin:10px\"><INPUT TYPE=\"checkbox\" NAME=\"shops\"  onchange=\"check(this)\">品牌：G7咖啡</div>";
				cart.appendChild(newForm);
			}
			for(i = 0; i < data.list.length; i++) {
				var newDiv = document.createElement("div");
				newDiv.classList.add("outter");
				newDiv.id = data.list[i].gid;
				newDiv.name = "item";
				newDiv.innerHTML =
					"				<div class=\"div1\" >" +
					"					<INPUT TYPE=\"checkbox\" name=\"goods\" style=\"vertical-align:top\"  onchange=\"check2(this)\">" +
					"					<IMG SRC=\"img/detail/" + data.list[i].pic + "\" WIDTH=\"79\" HEIGHT=\"80\" BORDER=\"0\" ALT=\"\" onmouseout=\"hidepic(this)\" onmouseover=\"showpic(this)\">" +
					"					<IMG class=\"itempic\" SRC=\"img/detail/" + data.list[i].pic + "\" WIDTH=\"240\" HEIGHT=\"205\" BORDER=\"0\" ALT=\"\">" +
					"				<span  style=\"font:12px;width:100px;vertical-align:top;display:inline-block\"><A HREF=\"\">" + data.list[i].gname + "</A></span>" +
					"				</div>" +
					"				<div class=\"div2\" style=\"color:gray;font-size:12px\">特浓速溶咖啡粉三合一90条装</div>" +
					"				<div class=\"div3\">" + data.list[i].price + "</div>" +
					"				<div class=\"div4\"><input type=\"number\" name=\"num\" value=\"1\" style=\"width:60px;text-align:center\" onchange=\"count(this)\"></div>" +
					"				<div class=\"div5\">" + data.list[i].price + "</div>" +
					"				<div class=\"div6\" style=\"font-size:12px;text-align:center;line-height:20px\"><A HREF=\"\">移入收藏夹</A><br><A HREF=\"#\" onclick=\"delFromCartList(\'" + data.list[i].gid + "\')\">删除</A></div>" +
					"				<div class=\"div7\"></div>";
				newForm.appendChild(newDiv);

			}
		}
		//根据数量计算金额
		function count(x) {

			var price = $(x).parent().prev().html();
			var num = $(x).val();
			var n=price*num;
			$(x).parent().next().html(n.toFixed(2));
			calculate();
		}
		//统计件数、金额
		function calculate() {
			var piece = 0;
			var money = 0;
			var o = document.getElementsByTagName("input");
			for(i = 0; i < o.length; i++) {
				if(o[i].type == "checkbox") {
					if(o[i].checked && o[i].name == "goods") {
						var t = $(o[i]).parent().nextAll().eq(3).html();
						piece += 1;
						money += Number(t);
					}
				}
			}
			$("#piece").html(piece);
			$("#money").html(money.toFixed(2));

		}
		//全选按钮 
		function checkAll(e) {

			var o = document.getElementsByTagName("input");
			for(i = 0; i < o.length; i++) {
				if(o[i].type == "checkbox") {
					if(e.checked) {
						o[i].checked = true;
					} //全选
					else {
						o[i].checked = false;
					} //取消全选
				}
			}

			calculate();
		}
		//店铺全选按钮
		function check(e) {
			var o = e.form.goods;
			for(i = 0; i < o.length; i++) {
				if(e.checked) {
					o[i].checked = true;
				} //全选
				else {
					o[i].checked = false;
				} //取消全选

			}

			calculate();
		}
		//选择商品按钮
		function check2(e) {
			if(e.checked) {
				var j = 0;
				var o = e.form.goods;
				for(i = 0; i < o.length; i++) {
					if(o[i].checked) {
						j++;
					} else break;
				}
				if(j == o.length) {
					e.form.shops.checked = true;
				}
			} else
				e.form.shops.checked = false;
			calculate();
		}
		//显示大图
		function showpic(p) {
			alert(p.nextSibling)
			var o = p.sibling();
			
		}
		//隐藏大图
		function hidepic(p) {
			var o = $(p).next().css("display", "none");
		}

		//删除商品
		function delFromCart(gid) {
			var y = confirm("确定要删除此商品？");
			if(y == true) {

				var goods = document.getElementById(gid);

				var shop = goods.parentNode;
				//删除商品
				shop.removeChild(goods);
				//如果店铺空了，则删除店铺         
				if(shop.children.length == 1) {
					shop.parentNode.removeChild(shop);
				}
			}
			calculate();
		}

		//删除本地存储商品
		function delFromCartList(gid) {

			delFromCart(gid);
			var str = localStorage.getItem("cartlist");
			var data = JSON.parse(str);
			for(i = 0; i < data.list.length; i++) {

				if(data.list[i].gid == gid) {

					data.list.splice(i, 1);
				}
			}

			var str = JSON.stringify(data);

			localStorage.setItem("cartlist", str);
		}