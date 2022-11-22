
select Id,OrderDate,PreviousOrderDate,round(julianday(OrderDate)-julianday(PreviousOrderDate),2)
from(select Id,OrderDate,lag(OrderDate,1,(select min(OrderDate) from 'order' where CustomerId = 'BLONP')) over (order by OrderDate) PreviousOrderDate
from 'Order'
where CustomerId = 'BLONP'
order by OrderDate
limit 10 )


