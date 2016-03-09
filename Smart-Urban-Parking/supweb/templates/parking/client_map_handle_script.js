
	$(function(){
	
		/*****Variables initialization ******/
		
		var created_parking;
		var state;
		var newlamp_marker;
 		var marker;
 		var circlemarker;
		var lampByID = {};
		var parkingByID = {};
		var _lamp = null;
		var _parking = null;
		var _marker = null;
		var position = null;
		var html_element = null;
		
		/*********Map initialization************/
		var map = L.map('map').setView([{% if lat and lon%} {{ lat }} , {{ lon }} {% else %} 0,0 {% endif %}], {{area_type}});

		L.tileLayer('http://{s}.tile.osm.org/{z}/{x}/{y}.png', {
    		attribution: '&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
		}).addTo(map);
		
		var info = L.control();

		info.onAdd = function (map) {
		    this._div = L.DomUtil.create('div', 'info'); // create a div with a class "info"
		    this.update(html_element);
		    return this._div;
		};

		// method that we will use to update the control based on feature properties passed
		info.update = function (element) {
			if(element=="zoom"){
			    this._div.innerHTML = '<h4>Select Area to zoom in</h4>' +'<select id="area_list" >'
			    {% if area_list %}
			    	{% for area_name in area_list %}
						+'<option value="{{area_name}}">{{area_name}}</option>'
					{% endfor %}
				{% endif %}
				+'</select>';
			}
			else if(element=="info"){
				this._div.innerHTML = '<h4>Click parking to display occupancy and capacity</h4>';
			}
		};
		html_element="info";
		info.addTo(map);
		html_element="zoom";
		info.addTo(map);
        /* Map events initialization ******/
		map.on('click', OnMapClick);
		
        
        /****Initialize lamps at the map ********/
		{% for lamp in lamp_points %}
			{% if lamp %}
				lampByID["{{ lamp.id }}"] = {
					id: "{{ lamp.id }}",
    				max_range: "{{lamp.max_range}}",
    				lat: {{ lamp.geom.centroid.y }},
    				lng: {{ lamp.geom.centroid.x }}
				};
			
 				
    			_lamp = lampByID["{{ lamp.id }}"];
    			
    			
			{% endif %}
		{% endfor %}
		
		/****Initialize parking at the map ********/
		{% for parking in parking_points %}
			{% if parking %}
				parkingByID["{{ parking.id }}"] = {
					id: "{{ parking.id }}",
    				lamp_location: "{{parking.lamp_location}}",
    				capacity: {{ parking.capacity }},
    				geom: {{ parking.geom.geojson|safe }}
				};
			
 				/*** For each parking add polygon****///
    			_parking = parkingByID["{{ parking.id }}"];
				var polygon = L.geoJson(_parking.geom).addTo(map);
				polygon.setStyle({color: "green"});
				polygon.lamp_location = _parking.lamp_location;
				polygon.id = _parking.id;
        		polygon.on('click', onParkingClick);
				//alert(JSON.stringify(_parking.geom))
			{% endif %}
		{% endfor %}
		
		/*********** end of initialization **************/
		
		/*********** Area drop down list handle ******/
		var area_list = document.getElementById("area_list");
		/**On each change of dropdown list obtain zoom details***/
		area_list.onchange = function() {
			var selection = this.value;	
			$.ajax({
				"type": "GET",
				"url": '{% url 'parking:area_get' %}',
				"data": { 'content' : JSON.stringify(selection) }, 
	    		"dataType": "json",
				"success": function(response){
					if (response["HTTPRESPONSE"] == 1){
						map.setView([response["lat"],response["lon"]],response["area_type"]);
					}
					else
					{
						alert("error "+response["HTTPRESPONSE"])
					}
				},
				error : function(xhr,errmsg,err) {
					alert("server error "+ xhr.status + ": " + xhr.responseText);
				}
			});		
		}
 		
 		/****** Map handles *****************/
 		function OnMapClick(map_point){
    		
    		map_handling("init");
    		
		}
		
		function onParkingClick(parkingclick) {
			_parking = parkingByID[parkingclick.target.id];
			
			parking_lamp = lampByID[_parking.lamp_location];
    		
	    		$.ajax({
					"type": "GET",
					"url": '{% url 'parking:parking_get' %}',
					"data": { 'content' : JSON.stringify(_parking.id) }, 
		    		"dataType": "json",
					"success": function(response){
						if (response["HTTPRESPONSE"] == 1){
							var popLocation= new L.LatLng(parking_lamp.lat,parking_lamp.lng);
							var popup = L.popup()
	    						.setLatLng(popLocation)
	    						.setContent('<b>Occupancy: ' + response["parking"]+'<br />Capacity: ' + _parking.capacity +'</b>')
	    						.openOn(map);
						}
						else
						{
							alert("error "+response["HTTPRESPONSE"])
						}
					},
					error : function(xhr,errmsg,err) {
						alert("server error "+ xhr.status + ": " + xhr.responseText);
					}
				});
    	}
		
	});


