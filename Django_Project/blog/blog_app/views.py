from django.template import loader, Context
from django.http import HttpResponse
from blog_app.models import BlogPost

# Create your views here.
def archive(request):
    posts = BlogPost.objects.all()
    c = Context({'posts': posts})
    t = loader.get_template("archive.html")
    return HttpResponse(t.render(c))
