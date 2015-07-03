$(document).ready(function() {
    var xmlhttp = new XMLHttpRequest();
	xmlhttp.open("GET","/list",true);
	xmlhttp.responseType = 'json';
	xmlhttp.send();
	xmlhttp.onreadystatechange=function() {
	   if (xmlhttp.readyState==4 && xmlhttp.status==200){
		  var mylist = (xmlhttp.response);
		 graph(mylist.list);
	   }
    }
});


Highcharts.createElement('link', {
   href: '//fonts.googleapis.com/css?family=Signika:400,700',
   rel: 'stylesheet',
   type: 'text/css'
}, null, document.getElementsByTagName('head')[0]);


function graph(list) {
	chart = new Highcharts.Chart({
		chart: {
			renderTo: 'container',
			animation: Highcharts.svg,
			type: 'spline',
			marginRight: 10,
			events: {
				load: function(){
					var series = this.series[0];
					var index = 10;
					setInterval(function(){
						var xmlhttp = new XMLHttpRequest();
						var list;
						xmlhttp.open("GET","/list",true);
						xmlhttp.responseType = 'json';
						xmlhttp.send();
						xmlhttp.onreadystatechange=function() {
							if (xmlhttp.readyState==4 && xmlhttp.status==200){
								mylist = (xmlhttp.response);
								list = mylist.list;
								var sum = 0;
								for(var i = 0; i<list.length ; i++)
								{
									sum += list[i].power;
								}
								document.getElementById("Average").innerHTML = "<span>Average Power = " + Math.round((sum/list.length)*100)/100 + " watts</span>";
								console.log(sum)
								var x = (new Date()).getTime();
								if(index < list.length)
									var y = list[index].power;
								else
									var y = list[list.length - 1].power;
								series.addPoint([x,y],true,true);
								index += 1;
							}
						}
					},2000);
				}
			}
		},
		title: {
			text: 'Live Power Usage'
		},
		xAxis: {
			type: 'datetime',
			tickPixelInterval: 200,
		},
		yAxis: {
			title: {
				text: 'Power',
			},
		},
		legend: {
			enabled: false
		},
		exporting: {
			enabled: false
		},
		series: [{
			name: 'Time',
			data: (function () {
				var data = [],
				time = (new Date()).getTime(),i;
				for (i = 0; i < 10; i += 1) {
					data.push({
						x: time + i * 1000,
						y: list[i].power
					});
				}
			return data;
			}())
		}]
	});		
}
