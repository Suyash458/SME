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

// Add the background image to the container
Highcharts.wrap(Highcharts.Chart.prototype, 'getContainer', function (proceed) {
   proceed.call(this);
   this.container.style.background = 'url(http://www.highcharts.com/samples/graphics/sand.png)';
});


Highcharts.theme = {
   colors: ["#f45b5b", "#8085e9", "#8d4654", "#7798BF", "#aaeeee", "#ff0066", "#eeaaee",
      "#55BF3B", "#DF5353", "#7798BF", "#aaeeee"],
   chart: {
      backgroundColor: null,
      style: {
         fontFamily: "Signika, serif"
      }
   },
   title: {
      style: {
         color: 'black',
         fontSize: '16px',
         fontWeight: 'bold'
      }
   },
   subtitle: {
      style: {
         color: 'black'
      }
   },
   tooltip: {
      borderWidth: 0
   },
   legend: {
      itemStyle: {
         fontWeight: 'bold',
         fontSize: '13px'
      }
   },
   xAxis: {
      labels: {
         style: {
            color: '#6e6e70'
         }
      }
   },
   yAxis: {
      labels: {
         style: {
            color: '#6e6e70'
         }
      }
   },
   plotOptions: {
      series: {
         shadow: true
      },
      candlestick: {
         lineColor: '#404048'
      },
      map: {
         shadow: false
      }
   },

   // Highstock specific
   navigator: {
      xAxis: {
         gridLineColor: '#D0D0D8'
      }
   },
   rangeSelector: {
      buttonTheme: {
         fill: 'white',
         stroke: '#C0C0C8',
         'stroke-width': 1,
         states: {
            select: {
               fill: '#D0D0D8'
            }
         }
      }
   },
   scrollbar: {
      trackBorderColor: '#C0C0C8'
   },

   // General
   background2: '#E0E0E8'
   
};

// Apply the theme
Highcharts.setOptions(Highcharts.theme);

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
								var x = (new Date()).getTime();
								var y = list[20].power;
								series.addPoint([x,y],true,true);
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
