$(function(){
				var n=0;
				var timer;
				
				change();
				function change(){
					$('ol li').attr('class','');
					$('ol li').eq(n).attr('class','active');
					$('ul').stop().animate({top:-300*n},500);
				}
				
				$('ol li').mouseover(function(){
					clearInterval(timer);
					
					n=$(this).index();
					change();
				});
				
				$('#focus').mouseover(function(){
					clearInterval(timer);
				});
				$('#focus').mouseout(function(){
					atuoPlay();
				});
				
				
				atuoPlay();
				function atuoPlay(){
					timer=setInterval(function(){
						n++;
						
						if(n>$('ol li').length-1){
							n=0;
						}
						
						change();
					},2000);
				}
			});