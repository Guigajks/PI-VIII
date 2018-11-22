from django.db import models

# Create your models here.
class Mac(models.Model):
    mac_address = models.CharField(max_length=30, null=False, blank=False)