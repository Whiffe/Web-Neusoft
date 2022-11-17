
/*全局变量*/
         var time = "";
         var index = 1;
/*load function*/
         $(function () {
			 $(".que-m").find("p").addClass("p_hide");
             
			 //show  label
			$(".que-m").mouseenter(function(){
			 $(this).find("p").removeClass("p_hide");
				$(this).find("p").addClass("p_show");		
			 })
			//hide label  
				$(".que-m").mouseleave(function(){
				$(this).find("p").addClass("p_hide");	  
				})

			//banner
             showimg(index);          
			
         });

         function showimg(num) {
             index = num;          
             $("#banner_img_2 li").hide().stop(true,true).eq(index-1).fadeIn("slow");
             index = index + 1 > 4 ? 1 : index + 1; // 4 共4张
             time = setTimeout("showimg(" + index + ")", 3000);
			 }
 /*打开注册窗口*/
   function reg(){    window.open("register.html","mywin","top=150,left=300,width=400,height=300,menubar=no");   } 
/*打开登录窗口*/
   function login(){   window.open("login.html","mywin","top=150,left=300,width=400,height=300,menubar=no");   } 
  /*广告栏2*/
  var tId;
startScroll();
$(function(){
    $('.ad_cycle li').each(function(){
        $(this).click(function(){
            slideHere($(this));
        }).mouseover(function(){
            stopHere($(this));
        }).mouseout(function(){
            startScroll();
        });
    });
});

  