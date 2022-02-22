docker:
	docker build -t sentione/polem-enrich:0.2 .

run:
	docker run -p 5000:5000 sentione/polem-enrich:0.2
