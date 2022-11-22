select *
from 
(select Id,ShipCountry,'NorthAmerica'
from 'Order'
where ShipCountry = 'USA' or ShipCountry = 'Mexico' or ShipCountry = 'Canada' 
union
select Id,ShipCountry,'OtherPlace'
from 'Order'
where ShipCountry <> 'USA' and ShipCountry <> 'Mexico' and ShipCountry <> 'Canada' )
where Id>15444
order by Id asc
limit 20

